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

#pragma once

#include "ball.h"
#include "graphics/objects/model.h"
#include "graphics/primitives/camera.h"

struct PlayerBall : public PoolBall {
    PlayerBall(const glm::vec3& position);

    void phys_tick(double delta_time) override;
    void draw_tick(double delta_time, double subtick_time = 0.0) override;

    Camera& get_camera() { return camera_; }

    void set_input_lock(bool lock) { input_locked_ = lock; }
    bool get_input_lock() const { return input_locked_; }

   private:
    void process_input();
    glm::mat4 get_arrow_matrix() const;

    Subcomponent<StaticMesh> arrow_{SubcomponentNone};

    Camera camera_ = Camera(1.0);

    glm::vec3 charge_ = glm::vec3(0.0);
    bool is_charging_ = false;

    double start_mpos_x = 0;
    double start_mpos_y = 0;

    bool input_locked_ = false;

    static const float POWER_LIMIT;
};
