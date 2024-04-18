/**
 * @file events.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Global all-important events
 * @version 0.1
 * @date 2024-03-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "pipelining/event.hpp"

/**
 * @brief Event that gets triggered every physics/logics tick
 *
 * @param[in] delta_time time since the previous tick
 */
using TickEvent = Event<double>;

/**
 * @brief Event that gets triggered every subtick
 *
 * @param[in] delta_time time since the previous tick
 * @param[in] subtick_time time from the start of the tick
 */
using SubtickEvent = Event<double, double>;
