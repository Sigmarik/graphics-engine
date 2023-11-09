#include "world_timer.h"

clock_t WorldTimer::SIM_START_ = 0;

clock_t WorldTimer::get_time() {
    clock_t current = clock();
    return current - SIM_START_;
}

double WorldTimer::get_time_sec() { return (double)get_time() / 100000.0; }

WorldTimer::WorldTimer() { SIM_START_ = clock(); }
