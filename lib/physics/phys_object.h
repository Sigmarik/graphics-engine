/**
 * @file phys_object.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Physical object class
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <glm/vec3.hpp>

#include "level_geometry.h"

struct PhysObject {
    virtual ~PhysObject() = default;

    virtual void tick(const LevelGeometry& geometry, double delta_time) = 0;

    virtual glm::vec3 get_position() const = 0;
    virtual glm::vec3 get_rotation() const = 0;

    virtual glm::vec3 get_interp_pos([[maybe_unused]] double time) const {
        return get_position();
    }

    virtual glm::vec3 get_interp_rot([[maybe_unused]] double time) const {
        return get_rotation();
    }
};
