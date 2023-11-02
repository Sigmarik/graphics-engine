#include "world_timer.h"

clock_t WorldTimer::SIM_START_ = 0;

clock_t WorldTimer::get_time() {
    clock_t current = clock();
    return current - SIM_START_;
}

double WorldTimer::get_time_sec() { return get_time() / CLOCKS_PER_SEC; }

WorldTimer::WorldTimer() { SIM_START_ = clock(); }
