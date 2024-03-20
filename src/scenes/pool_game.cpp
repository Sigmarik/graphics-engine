#include "pool_game.h"

#include "input/binary_input.h"
#include "managers/asset_manager.h"

PoolGame::PoolGame()
    : Scene(5.0, 1.0, 0.1),
      table_(*AssetManager::request<ComplexModel>("assets/models/table.obj")),

      contrast_vignette_(*AssetManager::request<Material>(
          "assets/materials/postprocessing/contrast_vignette.material.xml")),

      main_lamp_(glm::vec3(-1.0, 2.0, -1.2), glm::vec3(1.0, 1.0, 1.2)),
      sun_(glm::vec3(10.0, 7.0, 12.0), glm::vec3(1.5, 1.4, 1.0) * 40.0f),
      ambient_(glm::vec3(0.36, 0.36, 0.3)),

      player_(glm::vec3(0.0, POOL_BALL_RADIUS, 0.5)) {
    CollisionGroup level_colliders =
        *AssetManager::request<CollisionGroup>("assets/models/table.obj");

    for (const BoxCollider& collider : level_colliders) {
        get_collision().add_collider(collider);
    }

    get_renderer().track_object(contrast_vignette_);

    table_.spawn_self(*this);

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
