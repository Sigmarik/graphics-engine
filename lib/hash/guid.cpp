#include "guid.h"

#include <stdio.h>

#include "generation/noise.h"
#include "logger/logger.h"

GUID GUID::zero = (GUID){.left = 0, .right = 0};

GUID GUID::from_string(const std::string& string) {
    GUID guid;

    std::sscanf(string.c_str(), GUID_FMT_SCANF, &guid.left, &guid.right);

    return guid;
}

std::string GUID::to_string() const {
    char output[33] = "";
    std::sprintf(output, GUID_FMT_PRINTF, GUID_OUT(*this));
    return output;
}

GUID GUID::gen() {
    return (GUID){
        rand_uint64(),
        rand_uint64(),
    };
}
