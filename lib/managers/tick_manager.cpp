#include "tick_manager.h"

#include "logger/logger.h"
#include "time/world_timer.h"

//! WARNING: Linux-only implementation of usleep()
#include <unistd.h>

TickManager::
    TickManager(const std::function<void()>& input, const SimpleUpdate& physics,
                const ExterpUpdate& graphics)
    : update_input_(input), update_phys_(physics), update_graph_(graphics) {
    reset_timers();
}

void TickManager::reset_timers() {
    time_ = phys_time_ = WorldTimer::get_time_sec();
}

void TickManager::tick() {
    double time = WorldTimer::get_time_sec();
    delta_time_ = time - time_;
    time_ = time;

    if (fps_cap_ > 0 && 1.0 / fps_cap_ > delta_time_) {
        usleep((unsigned)((1.0 / fps_cap_ - delta_time_) * 1e6));
        delta_time_ = 1.0 / fps_cap_;
    }

    if (tps_ > 0) {
        slice_phys_tick();
    } else {
        update_input_();
        update_phys_(delta_time_);
        phys_time_ = time_;
    }

    update_graph_(delta_time_, time_ - phys_time_);

    if (fps_threshold_ * delta_time_ > 1.0) {
        log_printf(WARNINGS, "warning",
                   "Can not keep up with FPS requirement of %lg. Is the "
                   "visualization overloaded? (dt: %lg, fps: %lg)\n",
                   fps_threshold_, delta_time_, 1.0 / delta_time_);
    }

    ++tick_id_;
    age_ += delta_time_;
}

void TickManager::slice_phys_tick() {
    unsigned subtick_count = 0;

    double phys_dt = 1.0 / tps_;

    while (subtick_count < wormhole_threshold_ &&
           phys_time_ + phys_dt < time_) {
        ++subtick_count;

        phys_time_ += phys_dt;

        update_input_();
        update_phys_(phys_dt);
    }

    if (subtick_count >= wormhole_threshold_) {
        log_printf(WARNINGS, "warning",
                   "Can not keep up with TPS requirement of %u (made %u "
                   "iterations before aborting). Is the simulation "
                   "overloaded? (dt: %lg, fps: %lg)\n",
                   tps_, subtick_count, delta_time_, 1.0 / delta_time_);
        phys_time_ = time_;
    }
}

TickManager* GameLoop::manager_ = nullptr;
GameLoop GameLoop::instance_;

void GameLoop::run(TickManager& manager, std::function<bool()> stop_condition) {
    manager_ = &manager;

    while (!stop_condition()) {
        manager_->tick();
    }
}
