/**
 * @file ball_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Generic and player ball importers
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "generic_ball.h"
#include "logics/blueprints/component_importer.h"
#include "player_ball.h"

XML_BASED_IMPORTER(Producer, "ball") {
    glm::mat4 transform = demand<glm::mat4>(data, "transform", glm::mat4(1.0));

    bool player = request<bool>(data, "player", false);

    return new Asset<Producer>(PRODUCER {
        glm::vec3 position = to_point(parent_tform * transform);

        if (player) return Subcomponent<PlayerBall>(position);
        return Subcomponent<GenericBall>(position);
    });
}
