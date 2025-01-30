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

#pragma once

#include <inttypes.h>

#include <functional>
#include <set>

struct WorldTimer final {
    static uint64_t get_time();

    static uint64_t sec2ticks(double seconds);
    static double ticks2sec(uint64_t ticks);

    static double get_time_sec();

    static void schedule(double delay, const std::function<void()>& call);

    static void run_scheduled_calls();

   private:
    WorldTimer();

    struct ScheduledCall {
        uint64_t time = 0;
        std::function<void()> call{};

        bool operator<(const ScheduledCall& other) const {
            return time < other.time;
        }
    };

    static uint64_t SIM_START_;

    static std::set<ScheduledCall> scheduled_calls_;
};
