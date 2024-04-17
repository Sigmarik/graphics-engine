#include "ball.h"

#include <math.h>

#include "geometry/transforms.h"
#include "managers/asset_manager.h"
#include "src/scenes/pool_game.h"

PoolBall::PoolBall(const glm::vec3& position, const Model& model)
    : bouncer_(position, POOL_BALL_RADIUS) {
    shadow_ = new_child<PointLightComponent>(position, glm::vec3(-1.0) * 0.4f);
    model_ = new_child<StaticMesh>(model);

    shadow_->set_spread(0.0005f);
    shadow_->set_radius(0.028f);
}

void PoolBall::phys_tick(double delta_time) {
    bouncer_.tick(get_scene().get_collision(), delta_time);

    static const double FRICTION = 1.0;

    glm::vec3 velocity = get_velocity();

    if (glm::length(velocity) < FRICTION * delta_time) {
        set_velocity(glm::vec3(0.0));
    } else {
        set_velocity(velocity -
                     glm::normalize(velocity) * (float)(FRICTION * delta_time));
    }
}

void PoolBall::draw_tick(double delta_time, double subtick_time) {
    glm::vec3 position = bouncer_.get_interp_pos(subtick_time);
    position.y = bouncer_.get_position().y;

    shadow_->set_position(position);

    // clang-format off
    glm::mat4 transform = glm::mat4(1.0, 0.0, 0.0, 0.0,
                                    0.0, 1.0, 0.0, 0.0,
                                    0.0, 0.0, 1.0, 0.0,
                                    position.x, position.y, position.z, 1.0);
    // clang-format on

    model_->set_transform(transform);

    model_->set_hidden(!is_on_board());
}

void PoolBall::collide(PoolBall& ball) {
    if (glm::distance(get_position(), ball.get_position()) >
        POOL_BALL_RADIUS * 2.0)
        return;

    resolve_positions(ball);

    glm::vec3 velocity_diff = get_velocity() - ball.get_velocity();
    glm::vec3 position_diff = get_position() - ball.get_position();
    glm::vec3 avg_velocity = (get_velocity() + ball.get_velocity()) / 2.0f;

    set_velocity(avg_velocity +
                 reflect_plane(velocity_diff, position_diff) / 2.0f);
    ball.set_velocity(avg_velocity -
                      reflect_plane(velocity_diff, position_diff) / 2.0f);
}

bool PoolBall::is_moving() const {
    bool has_horiz_vel =
        glm::length(get_velocity() * glm::vec3(1.0, 0.0, 1.0)) > 1e-2;

    bool on_table = is_on_board();

    return on_table && has_horiz_vel;
}

bool PoolBall::is_on_board() const { return get_position().y > 0.0; }

void PoolBall::capture() { captured_pos_ = bouncer_.get_position(); }

void PoolBall::reset() {
    bouncer_.set_position(captured_pos_);
    bouncer_.set_velocity(glm::vec3(0.0));

    model_->set_hidden(false);
}

void PoolBall::begin_play(Scene& scene) {
    SceneComponent::begin_play(scene);

    receive_phys_ticks();
    receive_draw_ticks();
}

static float length2(const glm::vec3 vec) {
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

void PoolBall::resolve_positions(PoolBall& ball) {
    glm::vec3 velocity_diff = get_velocity() - ball.get_velocity();
    glm::vec3 position_diff = get_position() - ball.get_position();

    if (glm::length(velocity_diff) < 1e-3) {
        glm::vec3 avg_pos = (get_position() + ball.get_position()) / 2.0f;
        glm::vec3 delta =
            glm::normalize(position_diff) * (float)POOL_BALL_RADIUS;

        set_position(avg_pos + delta);
        ball.set_position(avg_pos - delta);
        return;
    }

    float collision_arm = length2(glm::cross(position_diff, velocity_diff)) /
                          length2(velocity_diff);
    float delta_arm = 4.0f * (float)POOL_BALL_RADIUS * (float)POOL_BALL_RADIUS -
                      collision_arm;
    float shift =
        glm::dot(velocity_diff, position_diff) / glm::length(velocity_diff);

    float delta_time = (sqrt(delta_arm) + shift) / glm::length(velocity_diff);

    set_position(get_position() - get_velocity() * delta_time);
    ball.set_position(ball.get_position() - ball.get_velocity() * delta_time);
}
