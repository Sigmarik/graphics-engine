#include "booleans.h"

const std::string& to_bool(bool value) {
    return value ? TRUE_STRING : FALSE_STRING;
}

bool get_bool(const std::string& string) { return string.length() > 0; }
