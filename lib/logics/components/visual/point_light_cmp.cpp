#include "point_light_cmp.h"

#include "logics/scene.h"

PointLightComponent::PointLightComponent(const glm::vec3& position,
                                         const glm::vec3& color)
    : position_(position), light_(color) {
    set_position(position_);
}

void PointLightComponent::spawn_self(Scene& scene) {
    scene.get_renderer().track_object(light_);

    SceneComponent::spawn_self(scene);
}

void PointLightComponent::set_position(const glm::vec3& position) {
    position_ = position;

    // clang-format off
    glm::mat4 transform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    position_.x, position_.y, position_.z, 1.0);
    // clang-format on

    light_.set_object_matrix(transform);
}
