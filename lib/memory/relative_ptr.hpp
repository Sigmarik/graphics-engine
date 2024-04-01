/**
 * @file relative_ptr.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Relative pointer class
 * @version 0.1
 * @date 2024-03-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <inttypes.h>

template <class T>
struct RelativePtr {
    RelativePtr(T* data, void* origin)
        : shift_(reinterpret_cast<const char*>(data) -
                 reinterpret_cast<const char*>(origin)) {}

    T& of(const void* origin) const {
        const char* ptr_origin = reinterpret_cast<const char*>(origin);
        return *(reinterpret_cast<T*>(ptr_origin + shift_));
    }

   private:
    long long int shift_;
};
