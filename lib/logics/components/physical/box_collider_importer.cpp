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

    return new Asset<Producer>([=](const glm::mat4& parent_tform) {
        glm::vec4 size_proto =
            parent_tform * transform * glm::vec4(1.0, 1.0, 1.0, 0.0);

        glm::vec3 size(size_proto.x, size_proto.y, size_proto.z);

        // clang-format off
        glm::mat4 descale(1.0 / size.x, 0.0, 0.0, 0.0,
                          0.0, 1.0 / size.y, 0.0, 0.0,
                          0.0, 0.0, 1.0 / size.z, 0.0,
                          0.0, 0.0, 0.0, 1.0);
        // clang-format on

        return Subcomponent<StaticBoxCollider>(BoxCollider(
            Box(glm::vec3(0.0), size), parent_tform * transform * descale));
    });
}
