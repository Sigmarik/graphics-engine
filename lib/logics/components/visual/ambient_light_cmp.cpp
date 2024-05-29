#include "ambient_light_cmp.h"

#include "logics/scene.h"

AmbientLightComponent::AmbientLightComponent(const glm::vec3& color)
    : light_(color) {}

void AmbientLightComponent::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    get_scene().get_renderer().track_object(light_);
}

void AmbientLightComponent::end_play(EndPlayReason reason) {
    if (has_scene()) {
        get_scene().get_renderer().untrack_object(light_);
    }
}
