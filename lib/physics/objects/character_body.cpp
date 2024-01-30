#include "character_body.h"

#include <math.h>

#include "geometry/transforms.h"
#include "physics/constants.h"

CharacterBody::CharacterBody(const glm::vec3& feet_pos, double width,
                             double height)
    : width_(width), height_(height) {
    double delta_height = (height_ - width_) / BODY_INTERP_LEVEL;

    for (unsigned id = 0; id < BODY_INTERP_LEVEL; ++id) {
        colliders_[id] = SphereCollider(
            width_ / 2.0,
            feet_pos + glm::vec3(0.0, width_ / 2.0 + delta_height * id, 0.0));
    }
}

void CharacterBody::tick(const LevelGeometry& level, double delta_time) {
    velocity_ += glm::vec3(0.0, -GRAVITY * delta_time, 0.0);

    glm::vec3 target = input_ * (float)movement_speed_;
    double grip = airborne_ ? air_control_multiplier_ : 1.0;
    double accelerator = acceleration_ * delta_time * grip;

    glm::vec3 proj_velocity = velocity_ * glm::vec3(1.0, 0.0, 1.0);

    if (glm::distance(target, proj_velocity) < accelerator) {
        velocity_ = target + glm::vec3(0.0, velocity_.y, 0.0);
    } else {
        velocity_ +=
            glm::normalize(target - proj_velocity) * (float)accelerator;
    }

    set_position(get_position() + velocity_ * (float)delta_time);

    glm::vec3 delta = glm::vec3(0.0);

    for (unsigned id = 0; id < BODY_INTERP_LEVEL; ++id) {
        delta += level.get_intersection(colliders_[id]);
    }

    if (glm::length(delta) < 1e-4) {
        airborne_ = true;
        return;
    }

    glm::vec2 projection =
        glm::vec2(sqrt(delta.x * delta.x + delta.z * delta.z), delta.y);

    if (projection.y > projection.x * stepup_slope_) {
        airborne_ = false;
    } else {
        airborne_ = true;
    }

    if (airborne_) {
        set_position(get_position() + delta);
        velocity_ = reflect_plane(velocity_, delta) * 0.2f;
    } else {
        set_position(get_position() + delta * glm::vec3(0.0, 1.0, 0.0));
        velocity_.y = 0.0f;
    }
}

void CharacterBody::set_position(const glm::vec3& position) {
    double delta_height = (height_ - width_) / BODY_INTERP_LEVEL;

    for (unsigned id = 0; id < BODY_INTERP_LEVEL; ++id) {
        colliders_[id].set_position(
            position + glm::vec3(0.0, width_ / 2.0 + delta_height * id, 0.0));
    }
}

void CharacterBody::jump(double strength, bool air_jump) {
    if (!airborne_ || air_jump) {
        velocity_.y += (float)strength;
    }
}
