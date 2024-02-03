#include "scene_component.h"

#include "logics/scene.h"

SceneComponent::SceneComponent() : guid_(GUID::gen()) {}

void SceneComponent::spawn_self(Scene& scene) {
    scene_ = &scene;
    scene_->add_component(*this);
}
