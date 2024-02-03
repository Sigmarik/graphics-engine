#include "bouncy_head.h"

#include "logics/scene.h"

BouncyHead::BouncyHead(const glm::vec3& position)
    : model_(*AssetManager::request<Model>("assets/models/monkey.model.xml")),
      bouncer_(position, 0.5) {}

void BouncyHead::spawn_self(Scene& scene) {
    scene.get_renderer().track_object(model_);

    SceneComponent::spawn_self(scene);
}

void BouncyHead::phys_tick(double delta_time) {
    assert(get_scene() != nullptr);

    bouncer_.tick(get_scene()->get_collision(), delta_time);
}

void BouncyHead::draw_tick(double delta_time, double subtick_time) {
    glm::vec3 position = bouncer_.get_interp_pos(subtick_time);

    // clang-format off
    glm::mat4 transform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    position.x, position.y, position.z, 1.0);
    // clang-format on

    model_.set_object_matrix(transform);
}
