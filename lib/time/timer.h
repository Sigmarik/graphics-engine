/**
 * @file timer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Timer class definition
 * @version 0.1
 * @date 2025-01-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <inttypes.h>

struct Timer final {
    Timer(double duration = 1.0);

    void set_duration(double duration);

    void start(double duration = -1.0);
    void pause();
    void unpause();
    void reset();

    bool running() const;
    bool finished() const;

    double elapsed_time() const;
    double remaining_time() const;

    double percentage() const;
    double duration() const;

   private:
    uint64_t timestamp_ = 0;
    uint64_t ts_progress_ = 0;
    uint64_t duration_ = 0;

    bool paused_ = true;
};
