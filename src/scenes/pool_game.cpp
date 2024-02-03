#include "pool_game.h"

#include "managers/asset_manager.h"

PoolGame::PoolGame()
    : Scene(5.0, 1.0, 0.1),
      field_(*AssetManager::request<Model>("assets/models/field.model.xml")),
      frame_(*AssetManager::request<Model>("assets/models/frame.model.xml")),
      main_lamp_(glm::vec3(-1.0, 2.0, -1.2), glm::vec3(1.0, 1.0, 1.2)),
      sun_(glm::vec3(10.0, 7.0, 12.0), glm::vec3(1.5, 1.4, 1.0) * 40.0f),
      ambient_(glm::vec3(0.36, 0.36, 0.3)),

      floor_(Box(glm::vec3(0.0, -0.05, 0.0), glm::vec3(1.05, 0.1, 2.1)),
             glm::mat4(1.0)),
      top_left_(Box(glm::vec3(1.05 / 2.0 + 0.5, 0.0, 0.935 / 2.0 + 0.05),
                    glm::vec3(1.0, 1.0, 0.935)),
                glm::mat4(1.0)),
      top_right_(Box(glm::vec3(1.05 / 2.0 + 0.5, 0.0, -0.935 / 2.0 - 0.05),
                     glm::vec3(1.0, 1.0, 0.935)),
                 glm::mat4(1.0)),
      bottom_left_(Box(glm::vec3(-1.05 / 2.0 - 0.5, 0.0, 0.935 / 2.0 + 0.05),
                       glm::vec3(1.0, 1.0, 0.935)),
                   glm::mat4(1.0)),
      bottom_right_(Box(glm::vec3(-1.05 / 2.0 - 0.5, 0.0, -0.935 / 2.0 - 0.05),
                        glm::vec3(1.0, 1.0, 0.935)),
                    glm::mat4(1.0)),
      left_top_(Box(glm::vec3(-0.41 / 2.0 - 0.05, 0.0, -2.1 / 2.0 - 0.5),
                    glm::vec3(0.41, 1.0, 1.0)),
                glm::mat4(1.0)),
      left_bottom_(Box(glm::vec3(0.41 / 2.0 + 0.05, 0.0, -2.1 / 2.0 - 0.5),
                       glm::vec3(0.41, 1.0, 1.0)),
                   glm::mat4(1.0)),
      right_top_(Box(glm::vec3(-0.41 / 2.0 - 0.05, 0.0, 2.1 / 2.0 + 0.5),
                     glm::vec3(0.41, 1.0, 1.0)),
                 glm::mat4(1.0)),
      right_bottom_(Box(glm::vec3(0.41 / 2.0 + 0.05, 0.0, 2.1 / 2.0 + 0.5),
                        glm::vec3(0.41, 1.0, 1.0)),
                    glm::mat4(1.0)),

      player_(glm::vec3(0.0, POOL_BALL_RADIUS, 0.5)) {
    BoxCollider* colliders[] = {
        &floor_,    &top_left_,    &top_right_, &bottom_left_,  &bottom_right_,
        &left_top_, &left_bottom_, &right_top_, &right_bottom_,
    };

    for (size_t id = 0; id < sizeof(colliders) / sizeof(*colliders); ++id) {
        get_collision().add_collider(*colliders[id]);
    }

    field_.spawn_self(*this);
    frame_.spawn_self(*this);

    player_.spawn_self(*this);

    main_lamp_.spawn_self(*this);
    sun_.spawn_self(*this);
    ambient_.spawn_self(*this);

    size_t ball_id = 0;
    for (unsigned x_id = 1; x_id <= 3; ++x_id) {
        for (unsigned y_id = 1; y_id <= x_id; ++y_id) {
            glm::vec3 position = glm::vec3(
                POOL_BALL_RADIUS * y_id * 2.0 - POOL_BALL_RADIUS * x_id,
                POOL_BALL_RADIUS, -POOL_BALL_RADIUS * x_id * 2.0 - 0.5);

            balls_[ball_id] = GenericBall(position);
            balls_[ball_id].spawn_self(*this);

            ++ball_id;
        }
    }

    get_renderer().set_viewpoint(&player_.get_camera());
}

void PoolGame::phys_tick(double delta_time) {
    Scene::phys_tick(delta_time);

    // TODO: Resolve ball-to-ball collisions
}
