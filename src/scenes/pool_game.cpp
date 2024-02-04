#include "pool_game.h"

#include "input/binary_input.h"
#include "managers/asset_manager.h"

PoolGame::PoolGame()
    : Scene(5.0, 1.0, 0.1),
      field_(*AssetManager::request<Model>("assets/models/field.model.xml")),
      frame_(*AssetManager::request<Model>("assets/models/frame.model.xml")),

      contrast_vignette_(*AssetManager::request<Material>(
          "assets/materials/postprocessing/contrast_vignette.material.xml")),

      main_lamp_(glm::vec3(-1.0, 2.0, -1.2), glm::vec3(1.0, 1.0, 1.2)),
      sun_(glm::vec3(10.0, 7.0, 12.0), glm::vec3(1.5, 1.4, 1.0) * 40.0f),
      ambient_(glm::vec3(0.36, 0.36, 0.3)),

      floor_(Box(glm::vec3(0.0, -0.05, 0.0), glm::vec3(1.05, 0.1, 2.1))),
      top_left_(Box(glm::vec3(1.05 / 2.0 + 0.5, 0.0, 0.935 / 2.0 + 0.05),
                    glm::vec3(1.0, 1.0, 0.935))),
      top_right_(Box(glm::vec3(1.05 / 2.0 + 0.5, 0.0, -0.935 / 2.0 - 0.05),
                     glm::vec3(1.0, 1.0, 0.935))),
      bottom_left_(Box(glm::vec3(-1.05 / 2.0 - 0.5, 0.0, 0.935 / 2.0 + 0.05),
                       glm::vec3(1.0, 1.0, 0.935))),
      bottom_right_(Box(glm::vec3(-1.05 / 2.0 - 0.5, 0.0, -0.935 / 2.0 - 0.05),
                        glm::vec3(1.0, 1.0, 0.935))),
      left_top_(Box(glm::vec3(-0.41 / 2.0 - 0.05, 0.0, -2.1 / 2.0 - 0.5),
                    glm::vec3(0.41, 1.0, 1.0))),
      left_bottom_(Box(glm::vec3(0.41 / 2.0 + 0.05, 0.0, -2.1 / 2.0 - 0.5),
                       glm::vec3(0.41, 1.0, 1.0))),
      right_top_(Box(glm::vec3(-0.41 / 2.0 - 0.05, 0.0, 2.1 / 2.0 + 0.5),
                     glm::vec3(0.41, 1.0, 1.0))),
      right_bottom_(Box(glm::vec3(0.41 / 2.0 + 0.05, 0.0, 2.1 / 2.0 + 0.5),
                        glm::vec3(0.41, 1.0, 1.0))),

      player_(glm::vec3(0.0, POOL_BALL_RADIUS, 0.5)) {
    BoxCollider* colliders[] = {
        &floor_,    &top_left_,    &top_right_, &bottom_left_,  &bottom_right_,
        &left_top_, &left_bottom_, &right_top_, &right_bottom_,
    };

    for (size_t id = 0; id < sizeof(colliders) / sizeof(*colliders); ++id) {
        get_collision().add_collider(*colliders[id]);
    }

    get_renderer().track_object(contrast_vignette_);

    field_.spawn_self(*this);
    frame_.spawn_self(*this);

    player_.spawn_self(*this);

    main_lamp_.spawn_self(*this);
    sun_.spawn_self(*this);
    ambient_.spawn_self(*this);

    for (size_t id = 0; id < sizeof(balls_) / sizeof(*balls_); ++id) {
        balls_[id] = GenericBall(glm::vec3(0.0));
        balls_[id].spawn_self(*this);
    }

    get_renderer().set_viewpoint(&player_.get_camera());

    reset();
}

void PoolGame::phys_tick(double delta_time) {
    Scene::phys_tick(delta_time);

    process_int_collisions();

    if (player_.get_position().y < -0.5) {
        reset();
    }

    player_.set_input_lock(has_moving_parts());

    static BinaryInput reset_input = *AssetManager::request<BinaryInput>(
        "assets/controls/reset.keybind.xml");

    if (reset_input.poll_pushed()) {
        reset();
    }
}

void PoolGame::reset() {
    size_t ball_id = 0;
    for (unsigned x_id = 1; x_id <= 3; ++x_id) {
        for (unsigned y_id = 1; y_id <= x_id; ++y_id) {
            glm::vec3 position = glm::vec3(
                POOL_BALL_RADIUS * y_id * 2.0 - POOL_BALL_RADIUS * x_id,
                POOL_BALL_RADIUS, -POOL_BALL_RADIUS * x_id * 1.73 - 0.5);

            balls_[ball_id].set_position(position);
            balls_[ball_id].set_velocity(glm::vec3(0.0));

            ++ball_id;
        }
    }

    player_.set_position(glm::vec3(0.0, POOL_BALL_RADIUS, 0.5));
    player_.set_velocity(glm::vec3(0.0));
}

bool PoolGame::has_moving_parts() const {
    for (size_t id = 0; id < sizeof(balls_) / sizeof(*balls_); ++id) {
        if (balls_[id].is_moving()) return true;
    }

    return player_.is_moving();
}

void PoolGame::process_int_collisions() {
    for (size_t id_a = 1; id_a < sizeof(balls_) / sizeof(*balls_); ++id_a) {
        for (size_t id_b = 0; id_b < id_a; ++id_b) {
            balls_[id_a].collide(balls_[id_b]);
        }
    }

    for (size_t id = 0; id < sizeof(balls_) / sizeof(*balls_); ++id) {
        player_.collide(balls_[id]);
    }
}
