/**
 * @file ambient_light_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Ambient light importer
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "ambient_light_cmp.h"
#include "logics/blueprints/component_importer.h"
#include "logics/blueprints/external_level.h"
#include "xml/data_extractors.h"

XML_BASED_IMPORTER(Producer, "ambient_light") {
    glm::vec3 color = demand<glm::vec3>(data, "color", glm::vec3(0.0));

    return new Asset<Producer>([=](const glm::mat4&) {
        return Subcomponent<AmbientLightComponent>(color);
    });
}
