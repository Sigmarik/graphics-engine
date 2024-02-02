#include "pool_game.h"

#include "managers/asset_manager.h"

PoolGame::PoolGame()
    : Scene(5.0, 1.0, 0.1),
      field_(*this,
             *AssetManager::request<Model>("assets/models/field.model.xml")),
      frame_(*this,
             *AssetManager::request<Model>("assets/models/frame.model.xml")),
      main_lamp_(*this, glm::vec3(-1.0, 2.0, -1.2), glm::vec3(1.0, 1.0, 1.2)),
      sun_(*this, glm::vec3(10.0, 7.0, 12.0), glm::vec3(1.5, 1.4, 1.0) * 40.0f),
      ambient_(*this, glm::vec3(0.36, 0.36, 0.3)),

      floor_(Box(glm::vec3(0.0, -0.05, 0.0), glm::vec3(1.05, 0.1, 2.1)),
             glm::mat4(1.0)),
      top_left_(Box(glm::vec3(1.05 / 2.0 + 0.5, 0.5, 0.935 / 2.0 + 0.05),
                    glm::vec3(1.0, 1.0, 0.935)),
                glm::mat4(1.0)),
      top_right_(Box(glm::vec3(1.05 / 2.0 + 0.5, 0.5, -0.935 / 2.0 - 0.05),
                     glm::vec3(1.0, 1.0, 0.935)),
                 glm::mat4(1.0)),
      bottom_left_(Box(glm::vec3(-1.05 / 2.0 - 0.5, 0.5, 0.935 / 2.0 + 0.05),
                       glm::vec3(1.0, 1.0, 0.935)),
                   glm::mat4(1.0)),
      bottom_right_(Box(glm::vec3(-1.05 / 2.0 - 0.5, 0.5, -0.935 / 2.0 - 0.05),
                        glm::vec3(1.0, 1.0, 0.935)),
                    glm::mat4(1.0)),
      left_top_(Box(glm::vec3(-0.41 / 2.0 - 0.05, 0.5, -2.1 / 2.0 - 0.5),
                    glm::vec3(0.41, 1.0, 1.0)),
                glm::mat4(1.0)),
      left_bottom_(Box(glm::vec3(0.41 / 2.0 + 0.05, 0.5, -2.1 / 2.0 - 0.5),
                       glm::vec3(0.41, 1.0, 1.0)),
                   glm::mat4(1.0)),
      right_top_(Box(glm::vec3(-0.41 / 2.0 - 0.05, 0.5, 2.1 / 2.0 + 0.5),
                     glm::vec3(0.41, 1.0, 1.0)),
                 glm::mat4(1.0)),
      right_bottom_(Box(glm::vec3(0.41 / 2.0 + 0.05, 0.0, 2.1 / 2.0 + 0.5),
                        glm::vec3(0.41, 1.0, 1.0)),
                    glm::mat4(1.0)),

      player_(*this, glm::vec3(0.0, POOL_BALL_RADIUS, 0.5)) {
    get_collision().add_collider(floor_);
    get_collision().add_collider(top_left_);
    get_collision().add_collider(top_right_);
    get_collision().add_collider(bottom_left_);
    get_collision().add_collider(bottom_right_);
    get_collision().add_collider(left_top_);
    get_collision().add_collider(left_bottom_);
    get_collision().add_collider(right_top_);
    get_collision().add_collider(right_bottom_);
}

void PoolGame::phys_tick(double delta_time) {
    Scene::phys_tick(delta_time);

    // TODO: Resolve ball-to-ball collisions
}
