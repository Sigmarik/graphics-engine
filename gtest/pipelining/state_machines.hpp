#include "pipelining/state_machine.h"

//! WARNING: Some incredibly bad code ahead, don't use it outside of state
//! machine testing
struct Weapon {
    int ammo = 1024;
    int clip = 32;

    struct ShootingState : StateMachine::State {
        ShootingState(Weapon& owner) : owner_(&owner) {}

        ShootingState(const ShootingState& instance) = default;
        ~ShootingState() = default;

        ShootingState& operator=(const ShootingState& instance) = default;

        void update(double) override { --owner_->clip; }

        Weapon* owner_;
    };

    struct ReloadingState : StateMachine::State {
        ReloadingState(Weapon& owner) : owner_(&owner) {}

        ReloadingState(const ReloadingState& instance) = default;
        ~ReloadingState() = default;

        ReloadingState& operator=(const ReloadingState& instance) = default;

        void enter() override { time_ = 0; }
        void update(double delta_time) override { time_ += delta_time; }
        void leave() override {
            owner_->clip = 32;
            owner_->ammo -= 32;
        }

        double time_ = 0.0;
        Weapon* owner_;
    };

    using IdleState = StateMachine::State;

    Weapon()
        : shooting_state(*this),
          reloading_state(*this),
          idle_state(),
          controller(idle_state) {
        controller.add_state(shooting_state);
        controller.add_state(reloading_state);

        controller.add_transition(idle_state, shooting_state,
                                  []() -> bool { return true; });
        controller.add_transition(shooting_state, reloading_state,
                                  [this]() -> bool { return clip <= 0; });
        controller.add_transition(
            reloading_state, idle_state,
            [this]() -> bool { return reloading_state.time_ > 1.0; });
    }

    ShootingState shooting_state;
    ReloadingState reloading_state;
    IdleState idle_state;

    StateMachine controller;
};

TEST(StateMachines, Weapon) {
    Weapon weapon;

    EXPECT_EQ(&weapon.controller.get_active(), &weapon.idle_state);

    weapon.controller.update(0.0);

    EXPECT_EQ(&weapon.controller.get_active(), &weapon.shooting_state);

    for (unsigned id = 0; id < 33; ++id) {
        weapon.controller.update(0.0);
    }

    EXPECT_EQ(&weapon.controller.get_active(), &weapon.reloading_state);

    EXPECT_EQ(weapon.clip, 0);

    weapon.controller.update(10.0);

    EXPECT_EQ(&weapon.controller.get_active(), &weapon.reloading_state);

    weapon.controller.update(0.0);

    EXPECT_EQ(&weapon.controller.get_active(), &weapon.idle_state);
    EXPECT_EQ(weapon.clip, 32);

    weapon.controller.update(0.0);

    EXPECT_EQ(&weapon.controller.get_active(), &weapon.shooting_state);
}
