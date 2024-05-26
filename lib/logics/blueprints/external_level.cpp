#include "external_level.h"

#include "logics/scene.h"

SubcomponentNameMap ExternalLevel::build(Scene& scene,
                                         const glm::mat4& transform) const {
    SubcomponentNameMap guide = factory_.build(transform);

    for (const auto& [name, component] : guide) {
        scene.add_component(component);
    }

    // TODO: Link pipeliner scripts and add them to the scene

    return guide;
}
