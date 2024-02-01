#include "player_ball.h"

#include <math.h>

PlayerBall::PlayerBall(Scene& scene, const glm::vec3& position)
    : PoolBall(scene, position) {
    camera_.set_position(glm::vec3(0.0, 4.0, 0.0));
    camera_.set_fov((float)(30.0));
    camera_.direct(glm::vec3(0.0, -1.0, 0.0), glm::vec3(1.0, 0.0, 0.0));

    scene.get_renderer().set_viewpoint(&camera_);
}

void PlayerBall::phys_tick(double delta_time) {
    PoolBall::phys_tick(delta_time);
}

void PlayerBall::draw_tick(double delta_time, double subtick_time) {
    PoolBall::draw_tick(delta_time, subtick_time);
}
