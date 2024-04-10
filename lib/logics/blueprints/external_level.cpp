#include "external_level.h"

#include "logics/scene.h"

void ExternalLevel::build(Scene& scene, const glm::mat4& transform) {
    SubcomponentNameMap guide = factory_.build(transform);

    for (auto& [name, component] : guide) {
        scene.add_component(component);
    }

    // TODO: Link pipeliner scripts and add them to the scene
}
