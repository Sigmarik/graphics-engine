#include "pool_game.h"

#include "managers/asset_manager.h"

PoolGame::PoolGame()
    : Scene(2.0, 1.0, 0.1),
      field_(*this,
             *AssetManager::request<Model>("assets/models/field.model.xml")),
      frame_(*this,
             *AssetManager::request<Model>("assets/models/frame.model.xml")),
      main_lamp_(*this, glm::vec3(-1.0, 2.0, -1.2), glm::vec3(1.0, 1.0, 1.2)),
      sun_(*this, glm::vec3(10.0, 7.0, 12.0), glm::vec3(1.5, 1.4, 1.0) * 40.0f),
      ambient_(*this, glm::vec3(0.36, 0.36, 0.3)),
      player_(*this, glm::vec3(0.0, 0.2, 0.5)) {}

void PoolGame::phys_tick(double delta_time) {
    Scene::phys_tick(delta_time);

    // TODO: Resolve ball-to-ball collisions
}
