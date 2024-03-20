/**
 * @file state_machine.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief State machine class
 * @version 0.1
 * @date 2024-03-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <map>
#include <vector>

struct StateMachine final {
    struct State {
        virtual ~State() = default;

        virtual void enter() {}
        virtual void update(double delta_time) {}
        virtual void leave() {}
    };

    StateMachine(State& start);

    StateMachine(const StateMachine&) = delete;
    StateMachine& operator=(const StateMachine&) = delete;

    StateMachine(const StateMachine&&) = delete;
    StateMachine& operator=(const StateMachine&&) = delete;

    void add_state(State& state);
    void add_transition(State& from, State& to,
                        std::function<bool()> condition);

    void update(double delta_time);

    State& get_active() const { return *active_state_; }

   private:
    using Transition = std::pair<State*, std::function<bool()>>;

    std::map<State*, std::vector<Transition>> map_ = {};

    State* active_state_;
};
