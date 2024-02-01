#include "ambient_light_cmp.h"

#include "logics/scene.h"

AmbientLightComponent::AmbientLightComponent(Scene& scene,
                                             const glm::vec3& color)
    : SceneComponent(scene), light_(color) {
    scene.get_renderer().track_object(light_);
}
