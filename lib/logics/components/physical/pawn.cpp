#include "pawn.h"

#include "logics/scene.h"

Pawn::Pawn(const glm::vec3& position, double width, double height)
    : body_(position, width, height) {}

void Pawn::phys_tick(double delta_time) {
    body_.tick(get_scene().get_collision(), delta_time);
}

void Pawn::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    receive_phys_ticks();
}
