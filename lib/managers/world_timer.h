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

#include "inttypes.h"

struct WorldTimer final {
    static uint64_t get_time();

    static double get_time_sec();

   private:
    WorldTimer();

    static uint64_t SIM_START_;
    static WorldTimer instance_;
};
