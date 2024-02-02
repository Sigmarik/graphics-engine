/**
 * @file constants.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Simulation physics constants
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __PHYSICS_CONSTANTS_H
#define __PHYSICS_CONSTANTS_H

#include "glm/vec3.hpp"

static const float GRAVITY = 9.8f;
static const float BOUNCINESS = 0.5f;

static const glm::vec3 DIRECTIONAL_BOUNCINESS = glm::vec3(1.0, 0.5, 1.0);

#endif
