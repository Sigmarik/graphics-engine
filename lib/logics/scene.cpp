#include "scene.h"

#include "logger/logger.h"

Scene::Scene(double width, double height, double cell_size)
    : collision_(Box(glm::vec3(0.0, 0.0, 0.0), glm::vec3(width, height, width)),
                 (size_t)(width / cell_size), (size_t)(height / cell_size)) {}

void Scene::add_component(SceneComponent& component) {
    components_.insert(&component);
}

void Scene::delete_component(SceneComponent& component) {
    auto address = components_.find(&component);

    if (address == components_.end()) {
        log_printf(WARNINGS, "warning",
                   "Deleting non-existent scene component " GUID_ES "\n",
                   GUID_OUT(component.get_guid()));
        return;
    }

    components_.erase(address);
}

void Scene::phys_tick(double delta_time) const {
    for (SceneComponent* component : components_) {
        assert(component);

        component->phys_tick(delta_time);
    }
}

void Scene::draw_tick(double delta_time, double subtick_time) const {
    for (SceneComponent* component : components_) {
        assert(component);

        component->draw_tick(delta_time, subtick_time);
    }
}
