/**
 * @file point_light_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Point light importer
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "logics/blueprints/component_importer.h"
#include "point_light_cmp.h"
#include "xml/data_extractors.h"

XML_BASED_IMPORTER(Producer, "point_light") {
    glm::mat4 transform = demand<glm::mat4>(data, "transform", glm::mat4(1.0));
    glm::vec3 color = demand<glm::vec3>(data, "color", glm::vec3(0.0));

    float spread = request<float>(data, "spread", 3.0);
    float radius = request<float>(data, "radius", 0.0);

    return new Asset<Producer>([=](const glm::mat4& root_transform) {
        glm::vec4 position =
            transform * root_transform * glm::vec4(0.0, 0.0, 0.0, 1.0);
        Subcomponent<PointLightComponent> light(glm::vec3(position), color);

        light->set_spread(spread);
        light->set_radius(radius);

        return light;
    });
}
