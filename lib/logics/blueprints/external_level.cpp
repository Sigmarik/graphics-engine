#include "external_level.h"

#include "logics/scene.h"
#include "logics/scene_component.h"

SubcomponentNameMap ExternalLevel::
    build(Scene& scene, const glm::mat4& transform) const {
    SubcomponentNameMap guide = factory_.build(transform);

    for (const auto& [name, component] : guide) {
        scene.add_component(component);
    }

    for (const Script& script : scripts_) {
        std::shared_ptr<Script> registered_script = scene.add_script(script);
        registered_script->assemble(scene, guide);
    }

    return guide;
}
