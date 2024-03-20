#include "state_machine.h"

StateMachine::StateMachine(State& start)
    : map_({{&start, {}}}), active_state_(&start) {
    active_state_->enter();
}

void StateMachine::add_state(State& state) { map_.insert({&state, {}}); }

void StateMachine::add_transition(State& from, State& to,
                                  std::function<bool()> condition) {
    map_[&from].push_back({&to, condition});
}

void StateMachine::update(double delta_time) {
    active_state_->update(delta_time);

    for (Transition& transition : map_[active_state_]) {
        if (!transition.second()) continue;

        active_state_->leave();
        active_state_ = transition.first;
        active_state_->enter();

        break;
    }
}
