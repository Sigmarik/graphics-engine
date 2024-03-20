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

/**
 * @brief State machine class, takes care of state switching
 *
 */
struct StateMachine final {
    /**
     * @brief State of a state machine
     *
     */
    struct State {
        virtual ~State() = default;

        /**
         * @brief Called whenever the state machine enters the state
         *
         */
        virtual void enter() {}

        /**
         * @brief Called every tick the machine is in this state
         *
         * @param[in] delta_time
         */
        virtual void update(double delta_time) {}

        /**
         * @brief Called whenever the machine leaves the state
         *
         */
        virtual void leave() {}
    };

    /**
     * @brief Construct a new State Machine object
     *
     * @param[in] start starting state of the machine
     */
    StateMachine(State& start);

    StateMachine(const StateMachine&) = delete;
    StateMachine& operator=(const StateMachine&) = delete;

    StateMachine(const StateMachine&&) = delete;
    StateMachine& operator=(const StateMachine&&) = delete;

    /**
     * @brief Register new state to the machine
     *
     * @param[in] state
     */
    void add_state(State& state);

    /**
     * @brief Add transition rule
     *
     * @param[in] from
     * @param[in] to
     * @param[in] condition
     */
    void add_transition(State& from, State& to,
                        std::function<bool()> condition);

    /**
     * @brief Update current state, switch states if necessary
     *
     * @param[in] delta_time
     */
    void update(double delta_time);

    /**
     * @brief Get current active state of the machine
     *
     * @return State&
     */
    State& get_active() const { return *active_state_; }

   private:
    using Transition = std::pair<State*, std::function<bool()>>;

    std::map<State*, std::vector<Transition>> map_ = {};

    State* active_state_;
};
