/**
 * @file world_timer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief World timer class
 * @version 0.1
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WORLD_TIMER_H
#define WORLD_TIMER_H

#include <time.h>

struct WorldTimer {
    static clock_t get_time();

    static double get_time_sec();

   private:
    WorldTimer();
    WorldTimer(const WorldTimer& timer) = default;
    WorldTimer& operator=(const WorldTimer& timer) = default;

    static WorldTimer& get_instance() {
        static WorldTimer timer;
        return timer;
    }

    static clock_t SIM_START_;
};

#endif
