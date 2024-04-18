#include "player_ball.h"

#include <math.h>

#include "input/binary_input.h"
#include "input/input_controller.h"
#include "managers/asset_manager.h"

const float PlayerBall::POWER_LIMIT = 2.5f;

PlayerBall::PlayerBall(const glm::vec3& position)
    : PoolBall(position, *AssetManager::request<Model>(
                             "assets/models/main_ball.model.xml")),
      arrow_(*AssetManager::request<Model>("assets/models/arrow.model.xml")) {
    camera_.set_position(glm::vec3(-1.0, 4.0, 0.0));
    camera_.set_fov((float)(30.0));
    camera_.direct(glm::vec3(0.25, -1.0, 0.0), glm::vec3(1.0, 0.0, 0.0));
}

void PlayerBall::begin_play(Scene& scene) {
    PoolBall::begin_play(scene);

    scene.get_renderer().track_object(arrow_);
}

void PlayerBall::phys_tick(double delta_time) {
    PoolBall::phys_tick(delta_time);

    if (!input_locked_) process_input();
}

void PlayerBall::draw_tick(double delta_time, double subtick_time) {
    if (glm::length(charge_) > 1e-4f) {
        arrow_.set_hidden(false);

        arrow_.set_object_matrix(get_arrow_matrix());
    } else {
        arrow_.set_hidden(true);
    }

    PoolBall::draw_tick(delta_time, subtick_time);
}

void PlayerBall::process_input() {
    static const BinaryInput* hit_input = AssetManager::request<BinaryInput>(
        "assets/controls/charge.keybind.xml");

    if (input_locked_) return;

    if (hit_input->poll_pushed() && !is_charging_) {
        start_mpos_x = InputController::get_mouse_x();
        start_mpos_y = InputController::get_mouse_y();

        is_charging_ = true;
    }

    if (hit_input->get_pushed() && is_charging_) {
        double delta_x = InputController::get_mouse_x() - start_mpos_x;
        double delta_y = InputController::get_mouse_y() - start_mpos_y;

        charge_ = glm::vec3(-(float)delta_y, 0.0, -(float)delta_x) / 100.0f;

        if (glm::length(charge_) > POWER_LIMIT) {
            charge_ = glm::normalize(charge_) * POWER_LIMIT;
        }
    }

    if (hit_input->poll_released() && is_charging_) {
        set_velocity(charge_);
        charge_ = glm::vec3(0.0);

        is_charging_ = false;
    }
}

glm::mat4 PlayerBall::get_arrow_matrix() const {
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 right = glm::normalize(glm::cross(charge_, up));

    glm::mat4 transform = glm::mat4(glm::vec4(-right, 0.0), glm::vec4(up, 0.0),
                                    glm::vec4(-glm::normalize(charge_), 0.0),
                                    glm::vec4(0.0, 0.0, 0.0, 1.0));
    transform =
        glm::translate(glm::mat4(1.0),
                       get_position() +
                           charge_ * 0.25f * glm::vec3(1.0, 0.0, 1.0) +
                           glm::vec3(0.0, -POOL_BALL_RADIUS + 0.002, 0.0)) *
        transform;

    return transform;
}
