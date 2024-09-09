/**
 * @file generic_meta.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Generic level metadata
 * @version 0.1
 * @date 2024-08-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "external_level.h"
#include "tinyxml2.h"

struct GenericMeta : public ExternalLevel::Metadata {
    GenericMeta(const tinyxml2::XMLElement& element);

    tinyxml2::XMLElement* xml() { return data_.FirstChildElement(); }
    const tinyxml2::XMLElement* xml() const {
        return data_.FirstChildElement();
    }

   private:
    tinyxml2::XMLDocument data_{};
};
