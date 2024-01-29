#include "bouncy_object.h"

#include <cstdio>

#include "geometry/transforms.h"
#include "physics/constants.h"

BouncyObject::BouncyObject(const glm::vec3& position, double radius)
    : collider_(radius, position) {}

void BouncyObject::tick(const LevelGeometry& level, double delta_time) {
    collider_.set_position(collider_.get_position() +
                           velocity_ * (float)delta_time);

    velocity_ += glm::vec3(0.0, -GRAVITY * (float)delta_time, 0.0);

    glm::vec3 intersection = level.get_intersection(collider_);
    if (glm::length(intersection) > 1e-4f) {
        collider_.set_position(collider_.get_position() + intersection);
        velocity_ = reflect_plane(velocity_, intersection) * BOUNCINESS;
    }
}
