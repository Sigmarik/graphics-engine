/**
 * @file shouter_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief ShouterComponent importer
 * @version 0.1
 * @date 2024-08-16
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "logics/blueprints/component_importer.h"
#include "shouter.h"
#include "xml/data_extractors.h"

XML_BASED_IMPORTER(Producer, "shouter") {
    const char* name = nullptr;
    data.QueryStringAttribute("name", &name);

    return new Asset<Producer>([=](const glm::mat4& parent_tform) {
        return Subcomponent<ShouterComponent>(name);
    });
}
