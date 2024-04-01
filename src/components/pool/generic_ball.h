/**
 * @file generic_ball.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Generic, non-player ball
 * @version 0.1
 * @date 2024-02-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "ball.h"

struct GenericBall : public PoolBall {
    GenericBall(const glm::vec3& position = glm::vec3(0.0, 0.0, 0.0));
};
