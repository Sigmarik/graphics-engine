#include "rc_head.h"

#include "input/binary_input.h"
#include "logics/scene.h"
#include "managers/asset_manager.h"

RCHead::RCHead(const glm::vec3& position)
    : Pawn(position, 0.3, 1.85),
      model_(*AssetManager::request<Model>("assets/models/monkey.model.xml")) {}

void RCHead::spawn_self(Scene& scene) {
    scene.get_renderer().track_object(model_);

    Pawn::spawn_self(scene);
}

void RCHead::phys_tick(double delta_time) {
    static BinaryInput* forward_in = AssetManager::request<BinaryInput>(
        "assets/controls/forward.keybind.xml");
    static BinaryInput* back_in =
        AssetManager::request<BinaryInput>("assets/controls/back.keybind.xml");
    static BinaryInput* left_in =
        AssetManager::request<BinaryInput>("assets/controls/left.keybind.xml");
    static BinaryInput* right_in =
        AssetManager::request<BinaryInput>("assets/controls/right.keybind.xml");
    static BinaryInput* jump_in =
        AssetManager::request<BinaryInput>("assets/controls/jump.keybind.xml");

    glm::vec3 input = glm::vec3(0.0);

    if (forward_in->get_pushed()) input += glm::vec3(1.0, 0.0, 0.0);
    if (back_in->get_pushed()) input += glm::vec3(-1.0, 0.0, 0.0);
    if (left_in->get_pushed()) input += glm::vec3(0.0, 0.0, 1.0);
    if (right_in->get_pushed()) input += glm::vec3(0.0, 0.0, -1.0);

    if (glm::length(input) > 1e-4) input = glm::normalize(input);

    if (jump_in->get_pushed()) get_body().jump(0.5);

    Pawn::move(input);

    Pawn::phys_tick(delta_time);
}

void RCHead::draw_tick(double delta_time, double subtick_time) {
    glm::vec3 position = get_body().get_interp_pos(subtick_time);

    position.y += 0.9f;

    // clang-format off
    glm::mat4 transform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    position.x, position.y, position.z, 1.0);
    // clang-format on

    model_.set_object_matrix(transform);
}
