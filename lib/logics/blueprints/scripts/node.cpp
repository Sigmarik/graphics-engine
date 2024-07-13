#include "node.h"

void Script::Node::subscribe_to(ChildReference other_node) {
    Update::Listener listener([this](Script::Node& node) {
        bool should_notify = update(node);
        if (should_notify) {
            trigger();
        }
    });

    other_node->get_update_event().subscribe(listener);

    listeners_.push_back(listener);
}
