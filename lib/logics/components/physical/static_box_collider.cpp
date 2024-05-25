#include "static_box_collider.h"

StaticBoxCollider::StaticBoxCollider(const BoxCollider& collider)
    : collider_(collider) {}

void StaticBoxCollider::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    scene.get_collision().add_collider(collider_);

    destroy();
}
