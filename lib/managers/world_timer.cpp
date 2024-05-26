#include "world_timer.h"

#include "GLFW/glfw3.h"

uint64_t WorldTimer::SIM_START_ = glfwGetTimerValue();
WorldTimer WorldTimer::instance_;

uint64_t WorldTimer::get_time() {
    uint64_t current = glfwGetTimerValue();
    return current - SIM_START_;
}

double WorldTimer::get_time_sec() {
    uint64_t time = get_time();
    uint64_t frequency = glfwGetTimerFrequency();

    if (frequency == 0)
        return 0;

    uint64_t whole = time / frequency;
    uint64_t remainder = time % frequency;

    return (double)whole + (double)remainder / (double)frequency;
}

WorldTimer::WorldTimer() { SIM_START_ = glfwGetTimerValue(); }
