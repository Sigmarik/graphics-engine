/**
 * @file box_collider_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Static box collider importer
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "logics/blueprints/component_importer.h"
#include "logics/components/physical/static_box_collider.h"
#include "xml/data_extractors.h"

XML_BASED_IMPORTER(Producer, "box_collider") {
    glm::mat4 transform = demand<glm::mat4>(data, "transform", glm::mat4(1.0));

    glm::vec3 size = demand<glm::vec3>(data, "size", glm::vec3(1.0));

    return new Asset<Producer>([=](const glm::mat4& parent_tform) {
        return Subcomponent<StaticBoxCollider>(
            BoxCollider(Box(glm::vec3(0.0), size), parent_tform * transform));
    });
}
