/**
 * @file tick_manager.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Tick manager class
 * @version 0.1
 * @date 2024-03-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <optional>

/**
 * @brief Simulation update scheduler
 *
 */
struct TickManager final {
    using SimpleUpdate = std::function<void(double)>;
    using ExterpUpdate = std::function<void(double, double)>;

    TickManager(const std::function<void()>& input, const SimpleUpdate& physics,
                const ExterpUpdate& graphics);

    void reset_timers();
    void tick();

    /**
     * @brief Set TPS requirement
     *
     * @param[in] tps required TPS (0 if should be synched with FPS)
     */
    void set_tps_req(unsigned tps) { tps_ = tps; }
    unsigned get_tps_req() const { return tps_; }

    /**
     * @brief Set framerate cap for the simulation
     *
     * @param[in] fps framerate cap
     */
    void set_fps_cap(unsigned fps) { fps_cap_ = fps; }
    unsigned get_fps_cap() const { return fps_cap_; }

    /**
     * @brief Set how many physics subticks should be performed before tick
     * manager slows down the simulation to fulfill the TPS requirement
     *
     * @param[in] threshold
     */
    void set_wormhole_threshold(unsigned threshold) {
        wormhole_threshold_ = threshold;
    }

    /**
     * @brief Set lowest expected FPS
     *
     * @param[in] threshold
     */
    void set_low_fps_threshold(unsigned threshold) {
        fps_threshold_ = threshold;
    }

    double get_real_tps() const { return tps_ > 0 ? tps_ : get_fps(); }
    double get_fps() const { return 1.0 / delta_time_; }

    unsigned long long get_tick_id() const { return tick_id_; }
    double get_age() const { return age_; }

   private:
    void slice_phys_tick();

    std::function<void()> update_input_;
    SimpleUpdate update_phys_;
    ExterpUpdate update_graph_;

    double time_ = 0.0;
    double phys_time_ = 0.0;

    unsigned tps_ = 60;
    unsigned fps_cap_ = 0;

    double delta_time_ = 0.0;

    unsigned wormhole_threshold_ = 100;
    double fps_threshold_ = 20.0;

    unsigned long long tick_id_ = 0;
    double age_ = 0.0;
};

/**
 * @brief Main loop of the program
 *
 */
struct GameLoop final {
    /**
     * @brief Run the simulation with the given tick manager until it is stopped
     *
     * @param[in] manager
     * @param[in] stop_condition
     */
    static void run(TickManager& manager, std::function<bool()> stop_condition);

    /**
     * @brief Get tick manager of the simulation (nullptr if stopped)
     *
     * @return TickManager*
     */
    static TickManager* get_manager() { return manager_; }

    /**
     * @brief Stop the simulation after the current tick ends
     *
     */
    static void stop() { manager_ = nullptr; }

    static bool is_running() { return manager_ != nullptr; }

   private:
    GameLoop() = default;

    static TickManager* manager_;
    static GameLoop instance_;
};
