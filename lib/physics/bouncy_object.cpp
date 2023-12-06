#include "bouncy_object.h"

BouncyObject::BouncyObject(const glm::vec3& position, double radius)
    : collider_(radius, position), position_(position) {}

void BouncyObject::tick(const LevelGeometry& geometry, double delta_time) {
    // TODO: Implement
}
