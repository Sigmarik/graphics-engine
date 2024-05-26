#include "bouncy_object.h"

#include <cstdio>

#include "geometry/transforms.h"
#include "physics/constants.h"

BouncyObject::BouncyObject(const glm::vec3& position, double radius)
    : collider_(radius, position) {}

void BouncyObject::tick(const LevelGeometry& level, double delta_time) {
    glm::vec3 old_velocity = velocity_;

    velocity_ += glm::vec3(0.0, -GRAVITY * (float)delta_time, 0.0);

    glm::vec3 old_pos = collider_.get_position();

    collider_.set_position(collider_.get_position() +
                           velocity_ * (float)delta_time);

    glm::vec3 intersection = level.get_intersection(collider_);
    if (glm::length(intersection) > 1e-4f) {
        collider_.set_position(collider_.get_position() + intersection);

        float delta_height = old_pos.y - collider_.get_position().y;
        if (delta_height < 0.0) delta_height = 0.0;

        // v^2 / 2 = gh
        float gravity_shift = glm::sqrt(2.0f * delta_height * GRAVITY);

        velocity_ = old_velocity + glm::vec3(0.0, gravity_shift, 0.0);

        velocity_ =
            reflect_plane(velocity_, intersection) * DIRECTIONAL_BOUNCINESS;
    }
}
