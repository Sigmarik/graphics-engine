/**
 * @file bouncy_object.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Bouncy physics object
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PHYSICS_BOUNCY_OBJECT_H
#define PHYSICS_BOUNCY_OBJECT_H

#include "physics/collider.h"
#include "physics/phys_object.h"

struct BouncyObject : public PhysObject {
    BouncyObject(const glm::vec3& position, double radius);

    void tick(const LevelGeometry& level, double delta_time) override;

    glm::vec3 get_position() const override { return collider_.get_position(); }
    glm::vec3 get_rotation() const override { return rotation_; }
    glm::vec3 get_velocity() const { return velocity_; }

    void set_position(const glm::vec3& pos) { collider_.set_position(pos); }
    void set_velocity(const glm::vec3& velocity) { velocity_ = velocity; }

    glm::vec3 get_interp_pos(double time) const override {
        return collider_.get_position() + velocity_ * (float)time * 0.0f;
    }

   private:
    SphereCollider collider_;

    glm::vec3 rotation_ = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 velocity_ = glm::vec3(0.0, 0.0, 0.0);
};

#endif
