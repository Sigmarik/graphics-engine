/**
 * @file subcomponent.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Subcomponent wrapper
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <concepts>
#include <iostream>
#include <memory>

struct SceneComponent;

/**
 * @brief `SceneComponent` wrapper for use in other components and scenes
 *
 * @note This wrapper is a limited equivalent of `std::shared_ptr`
 *
 * @tparam T component type
 */
template <class T>
requires std::derived_from<T, SceneComponent>
struct Subcomponent final {
    template <class U>
    friend struct Subcomponent;

    template <class U>
    friend struct WeakSubcomponent;

    template <class... Ts>
    requires std::constructible_from<T, Ts&&...> Subcomponent(Ts&&... args)
        : ptr_(std::make_shared<T>(args...)) {}

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_.operator->(); }

    template <class U>
    requires std::derived_from<U, T> Subcomponent(const Subcomponent<U>& other)
        : ptr_(other.ptr_) {}

    template <class U>
    requires std::derived_from<U, T> Subcomponent(Subcomponent<U>&& other)
        : ptr_(std::move(other.ptr_)) {}

    template <class U>
    requires std::derived_from<U, T> Subcomponent& operator=(
        const Subcomponent<U>& other) {
        ptr_ = other.ptr_;
        return *this;
    }

    template <class U>
    requires std::derived_from<U, T> Subcomponent& operator=(
        Subcomponent&& other) {
        ptr_ = std::move(other.ptr_);
        return *this;
    }

   private:
    template <class U>
    requires std::derived_from<U, T> Subcomponent(
        const std::shared_ptr<U>& component)
        : ptr_(component) {}

    std::shared_ptr<T> ptr_{};
};

/**
 * @brief Weak `SceneComponent` wrapper for use in other components and scenes
 *
 * @note This wrapper is a limited equivalent of `std::weak_ptr`
 *
 * @tparam T component type
 */
template <class T>
requires std::derived_from<T, SceneComponent>
struct WeakSubcomponent final {
    WeakSubcomponent() = default;

    template <class U>
    friend struct WeakSubcomponent;

    Subcomponent<T> lock() const { return Subcomponent<T>(ptr_.lock()); }
    bool expired() const { return ptr_.expired(); }

    template <class U>
    requires std::derived_from<U, T> WeakSubcomponent(
        const WeakSubcomponent<U>& other)
        : ptr_(other.ptr_) {}

    template <class U>
    requires std::derived_from<U, T> WeakSubcomponent(
        WeakSubcomponent<U>&& other)
        : ptr_(std::move(other.ptr_)) {}

    template <class U>
    requires std::derived_from<U, T> WeakSubcomponent(
        const Subcomponent<U>& other)
        : ptr_(other.ptr_) {}

    template <class U>
    requires std::derived_from<U, T> WeakSubcomponent& operator=(
        const WeakSubcomponent<U>& other) {
        ptr_ = other.ptr_;
        return *this;
    }

    template <class U>
    requires std::derived_from<U, T> WeakSubcomponent& operator=(
        WeakSubcomponent&& other) {
        ptr_ = std::move(other.ptr_);
        return *this;
    }

    template <class U>
    requires std::derived_from<U, T> WeakSubcomponent& operator=(
        const Subcomponent<U>& other) {
        ptr_ = other.ptr_;
        return *this;
    }

   private:
    std::weak_ptr<T> ptr_{};
};
