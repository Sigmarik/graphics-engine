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

#ifndef PHYS_GRID_H
#define PHYS_GRID_H

#include <glm/vec3.hpp>
#include <vector>

#include "collider.h"
#include "geometry/primitives.h"

struct LevelGeometry {
    LevelGeometry(Box bounding_box, size_t horiz_res, size_t vert_res);
    LevelGeometry(const LevelGeometry& geometry);
    ~LevelGeometry();

    LevelGeometry& operator=(const LevelGeometry& geometry);

    glm::vec3 get_intersection(const DynamicCollider& collider) const;

    void add_collider(const BoxCollider& collider);

   private:
    struct LevelCell {
        std::vector<const BoxCollider*> colliders = {};
        Box box = Box();
    };

    struct IndexBox {
        size_t low_x = 0, high_x = 0;
        size_t low_y = 0, high_y = 0;
        size_t low_z = 0, high_z = 0;
    };

    IndexBox find_indices(const Box& box) const;

    size_t horiz_res_ = 100;
    size_t vert_res_ = 30;

    Box boundary_;

    size_t get_cell_id(size_t id_x, size_t id_y, size_t id_z) const;

    LevelCell* cells_;

    std::vector<const BoxCollider*> global_colliders_ = {};
    std::vector<BoxCollider> colliders_ = {};
};

#endif
