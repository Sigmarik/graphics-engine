#include "ball.h"

#include "geometry/transforms.h"
#include "managers/asset_manager.h"
#include "src/scenes/pool_game.h"

PoolBall::PoolBall(Scene& scene, const glm::vec3& position, const Model& model)
    : SceneComponent(scene),
      model_(model),
      bouncer_(position, POOL_BALL_RADIUS),
      shadow_(scene, position, glm::vec3(-1.0) * 0.4f) {
    scene.get_renderer().track_object(model_);

    shadow_.set_spread(0.0005f);
    shadow_.set_radius(0.028f);
}

void PoolBall::phys_tick(double delta_time) {
    bouncer_.tick(get_scene().get_collision(), delta_time);
}

void PoolBall::draw_tick(double delta_time, double subtick_time) {
    glm::vec3 position = bouncer_.get_interp_pos(subtick_time);

    shadow_.set_position(position);

    // clang-format off
    glm::mat4 transform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    position.x, position.y, position.z, 1.0);
    // clang-format on

    model_.set_object_matrix(transform);
}

void PoolBall::collide(PoolBall& ball) {
    glm::vec3 velocity_diff = get_velocity() - ball.get_velocity();
    glm::vec3 position_diff = get_position() - ball.get_position();
    glm::vec3 avg_velocity = (get_velocity() + ball.get_velocity()) / 2.0f;

    if (glm::length(position_diff) < POOL_BALL_RADIUS * 2.0) return;

    set_position(get_position() + position_diff / 2.0f);
    ball.set_position(get_position() - position_diff / 2.0f);

    if (glm::dot(velocity_diff, position_diff) > 0.0) return;

    set_velocity(avg_velocity +
                 reflect_plane(velocity_diff, position_diff) / 2.0f);
    ball.set_velocity(avg_velocity -
                      reflect_plane(velocity_diff, position_diff) / 2.0f);
}
