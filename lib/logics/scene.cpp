#include "scene.h"

#include "logger/logger.h"

Scene::Scene(double width, double height, double cell_size)
    : collision_(Box(glm::vec3(0.0, 0.0, 0.0), glm::vec3(width, height, width)),
                 (size_t)(width / cell_size), (size_t)(height / cell_size)) {}

Scene::~Scene() {
    for_each_component([](SceneComponent& component) {
        component.destroy(SceneComponent::EndPlayReason::Quit);
    });
}

void Scene::for_each_component(
    std::function<void(SceneComponent&)> function) const {
    for (auto& [kay, value] : shared_components_) {
        function(*value);
    }

    for (auto& [kay, value] : static_components_) {
        if (value) function(*value);
    }
}

SceneComponent* Scene::get_component(GUID guid) {
    auto shared_found = shared_components_.find(guid);
    if (shared_found != shared_components_.end()) {
        return shared_found->second.operator->();
    }

    auto static_found = static_components_.find(guid);
    if (static_found != static_components_.end()) {
        return static_found->second;
    }

    return nullptr;
}

void Scene::delete_component(SceneComponent& component) {
    deletion_queue_.push_back(component.get_guid());
}

void Scene::process_deletions() {
    while (!deletion_queue_.empty()) {
        GUID deleted = deletion_queue_.front();
        deletion_queue_.pop_front();

        auto shared_found = shared_components_.find(deleted);
        if (shared_found != shared_components_.end()) {
            shared_components_.erase(shared_found);
            continue;
        }

        auto static_found = static_components_.find(deleted);
        if (static_found != static_components_.end()) {
            static_components_.erase(static_found);
            continue;
        }

        log_printf(
            ERROR_REPORTS, "error",
            "Deleting scene component not bound to the scene (GUID: " GUID_ES
            ")\n",
            GUID_OUT(deleted));
    }
}

std::shared_ptr<SceneComponent> Scene::add_component(
    std::shared_ptr<SceneComponent> component) {
    shared_components_.insert({component->get_guid(), component});

    component->begin_play(*this);

    return component;
}

void Scene::add_component(SceneComponent& component) {
    static_components_.insert({component.get_guid(), &component});

    component.begin_play(*this);
}

void Scene::phys_tick(double delta_time) {
    phys_tick_.trigger(delta_time);

    process_deletions();
}

void Scene::draw_tick(double delta_time, double subtick_time) {
    draw_tick_.trigger(delta_time, subtick_time);
}
