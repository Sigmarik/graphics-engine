/**
 * @file player_ball.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Player controlled ball
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __SRC_COMPONENTS_POOL_PLAYER_BALL_H
#define __SRC_COMPONENTS_POOL_PLAYER_BALL_H

#include "ball.h"
#include "graphics/primitives/camera.h"

struct PlayerBall : public PoolBall {
    PlayerBall(const glm::vec3& position);

    void phys_tick(double delta_time) override;
    void draw_tick(double delta_time, double subtick_time = 0.0) override;

    Camera& get_camera() { return camera_; }

   private:
    Camera camera_ = Camera(1.0);

    glm::vec3 charge_ = glm::vec3(0.0);

    double start_mpos_x = 0;
    double start_mpos_y = 0;
};

#endif /* __SRC_COMPONENTS_POOL_PLAYER_BALL_H */
