#include "timer.h"

#include <assert.h>

#include "world_timer.h"

Timer::Timer(double duration) {
    assert(duration >= 0.0);

    duration_ = WorldTimer::sec2ticks(duration);
}

void Timer::set_duration(double duration) {
    assert(duration >= 0.0);

    duration_ = WorldTimer::sec2ticks(duration);
}

void Timer::start(double duration) {
    reset();

    if (duration >= 0.0) {
        set_duration(duration);
    }

    paused_ = false;
}

void Timer::pause() {
    if (paused_) return;

    uint64_t time = WorldTimer::get_time();

    ts_progress_ += time - timestamp_;
    timestamp_ = time;
    paused_ = false;
}

void Timer::unpause() {
    if (!paused_) return;

    timestamp_ = WorldTimer::get_time();
    paused_ = false;
}

void Timer::reset() {
    timestamp_ = WorldTimer::get_time();
    ts_progress_ = 0;
}

bool Timer::running() const { return !paused_ && !finished(); }

bool Timer::finished() const { return elapsed_time() >= duration_; }

double Timer::elapsed_time() const {
    if (paused_) return ts_progress_;

    uint64_t time = WorldTimer::get_time() - timestamp_ + ts_progress_;

    if (time > duration_) {
        time = duration_;
    }

    return time;
}

double Timer::remaining_time() const { return duration_ - elapsed_time(); }

double Timer::percentage() const { return elapsed_time() / duration(); }

double Timer::duration() const { return WorldTimer::ticks2sec(duration_); }
