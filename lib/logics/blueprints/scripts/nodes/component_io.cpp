#include "component_io.h"

static SceneComponent* node_to_component(
    const Script::Node::ChildReference& node, Scene& scene) {
    std::optional<std::string> string = node->get_value();

    if (!string) return nullptr;

    GUID object_guid = GUID::from_string(string.value());

    return scene.get_component(object_guid);
}

nodes::OutputMethod::OutputMethod(ChildReference object, ChildReference method)
    : object_(object), method_(method) {}

bool nodes::OutputMethod::update(Node&) {
    Scene* scene = get_scene();

    assert(scene);

    std::optional<std::string> method_string = method_->get_value();
    if (method_string) return false;

    SceneComponent* component = node_to_component(object_, *scene);
    if (!component) return false;

    SceneComponent::Channel* channel =
        component->get_output(method_string.value());
    if (!channel) return false;

    update_listener_ =
        SceneComponent::Channel::Listener([this](const std::string& string) {
            set_value(string);
            trigger();
        });

    channel->subscribe(update_listener_);

    return false;
}

nodes::InputMethod::InputMethod(ChildReference object, ChildReference method,
                                ChildReference value)
    : object_(object), method_(method), value_(value) {
    subscribe_to(value_);
}

bool nodes::InputMethod::update(Node& initiator) {
    // Send the update event if only the value has changed.
    if (&initiator == value_.get()) {
        std::optional<std::string> string = value_->get_value();

        if (string.has_value()) output_.trigger(string.value());

        return true;
    }

    // Rewire the event otherwise.

    Scene* scene = get_scene();
    assert(scene);

    output_ = SceneComponent::Channel();

    std::optional<std::string> method_string = method_->get_value();
    if (!method_string) return false;

    SceneComponent* component = node_to_component(object_, *scene);
    if (!component) return false;

    SceneComponent::Channel::Listener* listener =
        component->get_input(method_string.value());
    if (!listener) return false;

    output_.subscribe(*listener);

    return false;
}
