/**
 * @file abstract_holder.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Abstract data holder structure
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <assert.h>

#include <functional>
#include <utility>

template <class T>
struct Abstract {
    template <class U>
    explicit Abstract(U* data)
        : data_(data),
          copy_function_([](T* ptr) -> T* { return new U(*(U*)ptr); }) {
        assert(data_);
    }

    Abstract(const Abstract<T>& other)
        : data_(other.copy_function_(other.data_)),
          copy_function_(other.copy_function_) {}
    Abstract(Abstract<T>&& other)
        : data_(other.data_), copy_function_(other.copy_function_) {
        other.data_ = nullptr;
    }

    Abstract<T>& operator=(const Abstract<T>& other) {
        delete data_;
        data_ = other.copy_function_(other.data_);
        copy_function_ = other.copy_function_;
        return *this;
    }

    Abstract<T>& operator=(Abstract<T>&& other) {
        delete data_;
        data_ = other.data_;
        other.data_ = nullptr;
        copy_function_ = std::move(other.copy_function_);
        return *this;
    }

    ~Abstract() { delete data_; }

    T* operator->() { return data_; }
    const T* operator->() const { return data_; }

    T& operator*() { return *data_; }
    const T& operator*() const { return *data_; }

    template <class U>
    U* as() {
        return dynamic_cast<U*>(data_);
    }

    template <class U>
    const U* as() const {
        return dynamic_cast<U*>(data_);
    }

   private:
    T* data_;

    using CopyFunction = std::function<T*(T*)>;

    CopyFunction copy_function_;
};
