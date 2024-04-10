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
    glm::vec3 position = demand<glm::vec3>(data, "position", glm::vec3(0.0));
    glm::vec3 color = demand<glm::vec3>(data, "color", glm::vec3(0.0));

    float spread = request<float>(data, "spread", 3.0);
    float radius = request<float>(data, "radius", 0.0);

    return new Asset<Producer>([=](const glm::mat4& transform) {
        Subcomponent<PointLightComponent> light(position, color);

        light->set_spread(spread);
        light->set_radius(radius);

        return light;
    });
}
