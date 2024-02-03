#include "ambient_light_cmp.h"

#include "logics/scene.h"

AmbientLightComponent::AmbientLightComponent(const glm::vec3& color)
    : light_(color) {}

void AmbientLightComponent::spawn_self(Scene& scene) {
    scene.get_renderer().track_object(light_);

    SceneComponent::spawn_self(scene);
}
