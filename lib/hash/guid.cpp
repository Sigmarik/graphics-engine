#include "guid.h"

#include <stdio.h>

#include "generation/noise.h"
#include "logger/logger.h"

GUID GUID::from_string(const std::string& string) {
    GUID guid;

    std::sscanf(string.c_str(), "%016lX%016lX", &guid.left, &guid.right);

    return guid;
}

std::string GUID::to_string() const {
    char output[33] = "";
    std::sprintf(output, GUID_FORMAT, GUID_OUT(*this));
    return output;
}

GUID GUID::gen() {
    return (GUID){
        rand_uint64(),
        rand_uint64(),
    };
}
