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

#include "collider.h"
#include "phys_object.h"

struct BouncyObject : public PhysObject {
    BouncyObject(const glm::vec3& position, double radius);

    void tick(const LevelGeometry& geometry, double delta_time) override;

    glm::vec3 get_position() const override { return position_; }
    glm::vec3 get_rotation() const override { return rotation_; }

   private:
    SphereCollider collider_;

    glm::vec3 position_ = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 rotation_ = glm::vec3(0.0, 0.0, 0.0);
};

#endif
