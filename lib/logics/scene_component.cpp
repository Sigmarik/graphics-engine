#include "scene_component.h"

#include "logger/logger.h"
#include "logics/scene.h"

SceneComponent::SceneComponent() : guid_(GUID::gen()) {}

bool SceneComponent::is_valid() const { return alive_ && scene_ != nullptr; }

Scene& SceneComponent::get_scene() const {
    if (scene_ == nullptr) {
        log_printf(
            ERROR_REPORTS, "error",
            "Requesting a scene reference from an unbound object (GUID " GUID_ES
            ")\n",
            GUID_OUT(guid_));
    }

    return *scene_;
}

void SceneComponent::destroy(EndPlayReason reason) {
    if (!alive_) return;

    alive_ = false;
    end_play(reason);

    destroyed_event_.trigger(reason);

    if (scene_) scene_->delete_component(*this);
}

SceneComponent::Channel* SceneComponent::get_output(const std::string& name) {
    auto found = outputs_.find(name);
    if (found == outputs_.end()) return nullptr;
    return &found->second.of(this);
}

SceneComponent::Channel::Listener* SceneComponent::get_input(
    const std::string& name) {
    auto found = inputs_.find(name);
    if (found == inputs_.end()) return nullptr;
    return &found->second.of(this);
}

void SceneComponent::begin_play(Scene& scene) {
    scene_ = &scene;
    spawned_event_.trigger(scene);
}

void SceneComponent::receive_phys_ticks() {
    phys_ticker_ = TickEvent::Listener(
        [this](double delta_time) { phys_tick(delta_time); });

    get_scene().get_phys_tick_event().subscribe(phys_ticker_);
}

void SceneComponent::receive_draw_ticks() {
    draw_ticker_ =
        SubtickEvent::Listener([this](double delta_time, double subtick_time) {
            draw_tick(delta_time, subtick_time);
        });

    get_scene().get_draw_tick_event().subscribe(draw_ticker_);
}

void SceneComponent::register_output(const std::string& name, Channel& output) {
    outputs_.insert({name, RelativePtr<Channel>(&output, this)});
}

void SceneComponent::register_input(const std::string& name,
                                    Channel::Listener& input) {
    inputs_.insert({name, RelativePtr<Channel::Listener>(&input, this)});
}

void SceneComponent::attach(Subcomponent<SceneComponent> child) {
    child->parent_destroyed_listener_ = Event<EndPlayReason>::Listener(
        [this](EndPlayReason reason) { destroy(reason); });

    child->parent_spawned_listener_ =
        Event<Scene&>::Listener([self = child](Scene& scene) {
            scene.add_component(self);
            self->begin_play(scene);
        });

    get_destroyed_event().subscribe(child->parent_destroyed_listener_);
    get_spawned_event().subscribe(child->parent_spawned_listener_);
}
