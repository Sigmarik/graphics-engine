#include "node.h"

void ScriptNode::subscribe_to(ChildReference other_node) {
    Update::Listener listener = [this](ScriptNode& node) {
        bool should_notify = refresh(node);
        if (should_notify) {
            trigger();
        }
    };

    other_node.lock().get()->get_update_event().subscribe(listener);

    listeners_.push_back(listener);
}
