#include "static_box_collider.h"

void StaticBoxCollider::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    scene.get_collision().add_collider(collider_);

    destroy();
}
