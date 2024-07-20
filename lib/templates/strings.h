/**
 * @file strings.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief String-related templates
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

template <size_t N>
struct StringLiteral final {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    char value[N];
};
