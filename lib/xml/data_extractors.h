/**
 * @file data_extractors.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief A collection of useful XML data extractors
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <tinyxml2.h>

#include <optional>

#include "logger/logger.h"

template <class T>
T extract(const tinyxml2::XMLElement* element);

template <class T>
std::optional<T> extract(const tinyxml2::XMLElement& root, const char* name) {
    const tinyxml2::XMLElement* element = root.FirstChildElement(name);

    if (element == nullptr) return {};

    return extract<T>(element);
}

template <class T>
T request(const tinyxml2::XMLElement& root, const char* name,
          const T& on_fail) {
    return extract<T>(root, name).value_or(on_fail);
}

template <class T>
T demand(const tinyxml2::XMLElement& root, const char* name, const T& on_fail) {
    auto result = extract<T>(root, name);

    if (!result) {
        log_printf(ERROR_REPORTS, "error", "Could not find tag \"%s\"\n", name);
        return on_fail;
    }

    return *result;
}
