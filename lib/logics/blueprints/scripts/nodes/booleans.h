#pragma once

#include <string>

[[maybe_unused]] static const std::string TRUE_STRING = "1";
[[maybe_unused]] static const std::string FALSE_STRING = "";

const std::string& to_bool(bool value);

bool get_bool(const std::string& string);
