#include "bouncy_object.h"

#include <cstdio>

#include "constants.h"
#include "geometry/transforms.h"

BouncyObject::BouncyObject(const glm::vec3& position, double radius)
    : collider_(radius, position), position_(position) {}

void BouncyObject::tick(const LevelGeometry& level, double delta_time) {
    position_ += velocity_ * (float)delta_time;
    velocity_ += glm::vec3(0.0, -GRAVITY * (float)delta_time, 0.0);

    glm::vec3 intersection = level.get_intersection(collider_);
    if (intersection.length() > 1e-4) {
        printf("Collision!\n");

        position_ += intersection;
        velocity_ = reflect_plane(velocity_, intersection) * BOUNCINESS;
    }
}
