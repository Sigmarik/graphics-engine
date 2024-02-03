#include "player_ball.h"

#include <math.h>

#include "input/binary_input.h"
#include "input/input_controller.h"
#include "managers/asset_manager.h"

PlayerBall::PlayerBall(const glm::vec3& position)
    : PoolBall(position, *AssetManager::request<Model>(
                             "assets/models/main_ball.model.xml")) {
    camera_.set_position(glm::vec3(-1.0, 4.0, 0.0));
    camera_.set_fov((float)(30.0));
    camera_.direct(glm::vec3(0.25, -1.0, 0.0), glm::vec3(1.0, 0.0, 0.0));
}

void PlayerBall::phys_tick(double delta_time) {
    PoolBall::phys_tick(delta_time);

    static const double FRICTION = 1.0;

    glm::vec3 velocity = get_velocity();

    if (glm::length(velocity) < FRICTION * delta_time) {
        set_velocity(glm::vec3(0.0));
    } else {
        set_velocity(velocity -
                     glm::normalize(velocity) * (float)(FRICTION * delta_time));
    }

    static BinaryInput* hit_input = AssetManager::request<BinaryInput>(
        "assets/controls/charge.keybind.xml");

    if (hit_input->poll_pushed()) {
        start_mpos_x = InputController::get_mouse_x();
        start_mpos_y = InputController::get_mouse_y();
    }

    if (hit_input->get_pushed()) {
        double delta_x = InputController::get_mouse_x() - start_mpos_x;
        double delta_y = InputController::get_mouse_y() - start_mpos_y;

        charge_ = glm::vec3(-(float)delta_y, 0.0, -(float)delta_x) / 100.0f;
        charge_.y = glm::length(charge_) * 0.1f;
    }

    if (hit_input->poll_released()) {
        set_velocity(charge_);
        charge_ = glm::vec3(0.0);
    }
}

void PlayerBall::draw_tick(double delta_time, double subtick_time) {
    set_model_shift(-charge_ / 10.0f);

    PoolBall::draw_tick(delta_time, subtick_time);
}
