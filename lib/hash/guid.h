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

#ifndef __HASH_GUID_H
#define __HASH_GUID_H

#include <inttypes.h>

#define GUID_ES "%08lX%08lX"
#define GUID_OUT(guid) (guid).left, (guid).right

struct GUID {
    static GUID gen();

    bool valid() const { return left > 0 || right > 0; }

    bool operator==(const GUID& guid) const {
        return left == guid.left && right == guid.right;
    }

    bool operator>(const GUID& guid) const {
        if (left != guid.left) {
            return left > guid.left;
        }
        return right > guid.right;
    }

    bool operator<(const GUID& guid) const { return guid.operator>(*this); }

    bool operator>=(const GUID& guid) const { return !operator<(guid); }

    bool operator<=(const GUID& guid) const { return !operator>(guid); }

    uint64_t left = 0;
    uint64_t right = 0;
};

#endif
