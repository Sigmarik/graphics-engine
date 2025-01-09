#include "world_timer.h"

#include <assert.h>

#include "GLFW/glfw3.h"

uint64_t WorldTimer::SIM_START_ = glfwGetTimerValue();
std::set<WorldTimer::ScheduledCall> WorldTimer::scheduled_calls_ = {};

uint64_t WorldTimer::get_time() {
    uint64_t current = glfwGetTimerValue();
    return current - SIM_START_;
}

uint64_t WorldTimer::sec2ticks(double seconds) {
    return (uint64_t)(seconds * glfwGetTimerFrequency());
}

double WorldTimer::ticks2sec(uint64_t ticks) {
    return (double)ticks / (double)glfwGetTimerFrequency();
}

double WorldTimer::get_time_sec() {
    uint64_t time = get_time();
    uint64_t frequency = glfwGetTimerFrequency();

    uint64_t whole = time / frequency;
    uint64_t remainder = time % frequency;

    return (double)whole + (double)remainder / (double)frequency;
}

void WorldTimer::schedule(double delay, const std::function<void()>& call) {
    assert(delay >= 0.0);

    ScheduledCall scheduled{};
    scheduled.call = call;
    scheduled.time =
        get_time() + (uint64_t)(delay * (double)glfwGetTimerFrequency());

    scheduled_calls_.insert(scheduled);
}

void WorldTimer::run_scheduled_calls() {
    uint64_t time = get_time();

    while (!scheduled_calls_.empty() &&
           time >= scheduled_calls_.begin()->time) {
        auto call = scheduled_calls_.begin();
        std::function<void()> functor = call->call;
        scheduled_calls_.erase(call);

        functor();
    }
}

WorldTimer::WorldTimer() { SIM_START_ = glfwGetTimerValue(); }
