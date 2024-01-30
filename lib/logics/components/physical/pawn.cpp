#include "pawn.h"

#include "logics/scene.h"

Pawn::Pawn(Scene& scene, const glm::vec3& position, double width, double height)
    : SceneComponent(scene), body_(position, width, height) {}

void Pawn::phys_tick(double delta_time) {
    body_.tick(get_scene().get_collision(), delta_time);
}
