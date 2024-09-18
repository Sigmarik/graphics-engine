/**
 * @file level_geometry.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Level geometry structure
 * @version 0.1
 * @date 2023-11-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "collider.h"
#include "geometry/primitives.h"
#include "geometry/static_box_field.hpp"

using CollisionGroup = std::vector<BoxCollider>;

struct LevelGeometry {
    LevelGeometry(Box bounding_box, size_t horiz_res, size_t vert_res);

    LevelGeometry& operator=(const LevelGeometry& geometry);

    glm::vec3 get_intersection(const DynamicCollider& collider) const;

    void add_collider(const BoxCollider& collider);

   private:
    StaticBoxField<BoxCollider> colliders_;
};
