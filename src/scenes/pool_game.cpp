#include "pool_game.h"

#include "input/binary_input.h"
#include "logics/blueprints/external_level.h"
#include "managers/asset_manager.h"

PoolGame::PoolGame()
    : Scene(5.0, 1.0, 0.1),

      contrast_vignette_(*AssetManager::request<Material>(
          "assets/materials/postprocessing/contrast_vignette.material.xml")),

      main_lamp_(glm::vec3(-1.0, 2.0, -1.2), glm::vec3(1.0, 1.0, 1.2)),
      sun_(glm::vec3(10.0, 7.0, 12.0), glm::vec3(1.5, 1.4, 1.0) * 40.0f) {
    CollisionGroup level_colliders =
        *AssetManager::request<CollisionGroup>("assets/models/table.obj");

    for (const BoxCollider& collider : level_colliders) {
        get_collision().add_collider(collider);
    }

    get_renderer().track_object(contrast_vignette_);

    add_component(main_lamp_);
    add_component(sun_);

    load();
}

void PoolGame::phys_tick(double delta_time) {
    Scene::phys_tick(delta_time);

    process_int_collisions();

    Subcomponent<PlayerBall> player = player_.lock();

    if (player->get_position().y < -0.5) {
        reset();
    }

    player->set_input_lock(has_moving_parts());

    static BinaryInput reset_input = *AssetManager::request<BinaryInput>(
        "assets/controls/reset.keybind.xml");

    if (reset_input.poll_pushed()) {
        reset();
    }
}

void PoolGame::reset() {
    for (WeakSubcomponent<SceneComponent>& component : loaded_components_) {
        if (component.expired()) continue;
        component.lock()->reset();
    }
}

void PoolGame::load() {
    const ExternalLevel* level = AssetManager::request<ExternalLevel>(
        "assets/levels/pool_table.level.xml");

    if (level == nullptr) return;

    SubcomponentNameMap map = level->build(*this, glm::mat4(1.0));

    for (auto& [name, component] : map) {
        loaded_components_.push_back(component);
        component->capture();
    }

    if (map.count("PlayerBall") == 0) {
        log_printf(ERROR_REPORTS, "error",
                   "Could not find the ball named \"PlayerBall\", which is "
                   "required for the game initialization. Terminating.\n");
        return;
    }

    player_ = dynamic_subcomponent_cast<PlayerBall>(map["PlayerBall"]);
    if (!player_) {
        log_printf(ERROR_REPORTS, "error",
                   "Invalid player \"PlayerBall\" component type\n");
    }

    for (auto& [name, component] : map) {
        auto ball = dynamic_subcomponent_cast<GenericBall>(component);

        if (!ball) {
            continue;
        }

        balls_.push_back(ball);
    }

    get_renderer().set_viewpoint(&player_.lock()->get_camera());
}

bool PoolGame::has_moving_parts() const {
    for (size_t id = 0; id < balls_.size(); ++id) {
        if (balls_[id].lock()->is_moving()) return true;
    }

    return player_.lock()->is_moving();
}

void PoolGame::process_int_collisions() {
    for (size_t id_a = 1; id_a < balls_.size(); ++id_a) {
        for (size_t id_b = 0; id_b < id_a; ++id_b) {
            balls_[id_a].lock()->collide(*balls_[id_b].lock());
        }
    }

    for (size_t id = 0; id < balls_.size(); ++id) {
        player_.lock()->collide(*balls_[id].lock());
    }
}
