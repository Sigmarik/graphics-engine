#include "component_pack.h"

#include "logics/scene.h"

void ComponentPack::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    for (auto component : components_) {
        scene.add_component(component);
    }

    destroy();
}
