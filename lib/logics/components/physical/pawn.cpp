#include "pawn.h"

#include "logics/scene.h"

Pawn::Pawn(const glm::vec3& position, double width, double height)
    : body_(position, width, height) {}

void Pawn::phys_tick(double delta_time) {
    assert(get_scene() != nullptr);

    body_.tick(get_scene()->get_collision(), delta_time);
}
