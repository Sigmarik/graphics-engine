/**
 * @file guid.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief GUID module
 * @version 0.1
 * @date 2023-12-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <inttypes.h>

#include <string>

#define GUID_FORMAT "%08lX%08lX"
#define GUID_OUT(guid) (guid).left, (guid).right

struct GUID {
    static GUID from_string(const std::string& string);
    std::string to_string() const;

    static GUID gen();

    bool valid() const { return left > 0 || right > 0; }

    friend bool operator==(const GUID& alpha, const GUID& beta) {
        return alpha.left == beta.left && alpha.right == beta.right;
    }

    friend bool operator!=(const GUID& alpha, const GUID& beta) {
        return !(alpha == beta);
    }

    friend bool operator<(const GUID& alpha, const GUID& beta) {
        return alpha.left < beta.left ||
               (alpha.left == beta.left && alpha.right < beta.right);
    }

    friend bool operator>(const GUID& alpha, const GUID& beta) {
        return alpha.left > beta.left ||
               (alpha.left == beta.left && alpha.right > beta.right);
    }

    friend bool operator<=(const GUID& alpha, const GUID& beta) {
        return alpha < beta || alpha == beta;
    }

    friend bool operator>=(const GUID& alpha, const GUID& beta) {
        return alpha > beta || alpha == beta;
    }

    uint64_t left = 0;
    uint64_t right = 0;
};
