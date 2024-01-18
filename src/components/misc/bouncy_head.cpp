#include "bouncy_head.h"

#include "logics/scene.h"

BouncyHead::BouncyHead(Scene& scene, const glm::vec3& position)
    : SceneComponent(scene),
      model_(*AssetManager::request<Model>("assets/models/monkey.model.xml")),
      bouncer_(position, 0.5) {
    scene.get_renderer().track_object(model_);
}

void BouncyHead::phys_tick(double delta_time) {
    bouncer_.tick(get_scene().get_collision(), delta_time);
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
