#include "guid.h"

#include <stdio.h>

#include "generation/noise.h"

GUID GUID::from_string(const std::string& string) {
    GUID guid;

    std::sscanf(string.c_str(), GUID_ES, &guid.left, &guid.right);

    return guid;
}

std::string GUID::to_string() const {
    static char output[64] = "";
    std::sprintf(output, GUID_ES, GUID_OUT(*this));
    return output;
}

GUID GUID::gen() {
    return (GUID){
        rand_uint64(),
        rand_uint64(),
    };
}
