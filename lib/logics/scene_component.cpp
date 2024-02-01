#include "scene_component.h"

#include "logics/scene.h"

SceneComponent::SceneComponent(Scene& scene)
    : guid_(GUID::gen()), scene_(scene) {
    scene.add_component(*this);
}
