/**
 * @file main.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Main module of the program
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <unistd.h>

#include "graphics/gl_debug.h"
#include "input/input_controller.h"
#include "io/main_io.h"
#include "logger/debug.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"
#include "managers/tick_manager.h"
#include "managers/window_manager.h"
#include "scenes/pool_game.h"
#include "utils/main_utils.h"

#define MAIN

#include "config.h"

int main(const int argc, char** argv) {
    atexit(log_end_program);

    Options options;

    log_init("program_log.html", LOG_THRESHOLD, &errno);
    print_label();

    log_printf(STATUS_REPORTS, "status", "Initializing\n");

    if (argp_parse(&ARG_P, argc, argv, 0, 0, &options) != 0) {
        return EXIT_FAILURE;
    }

    WindowManager::init(WINDOW_WIDTH, WINDOW_HEIGHT, "Pool game", false);

    poll_gl_errors();

    InputController::init(WindowManager::get_active_window());

    static PoolGame world;

    Camera* camera = world.get_renderer().get_viewpoint();

    if (camera != nullptr) {
        camera->set_aspect_ratio((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT);
    }

    poll_gl_errors();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    RenderBundle gbuffers(WINDOW_WIDTH, WINDOW_HEIGHT);

    poll_gl_errors();

    auto& fps_subtitle = WindowManager::add_subtitle_param("FPS");
    auto& tps_subtitle = WindowManager::add_subtitle_param("TPS");
    WindowManager::set_title_update_frequency(60);

    TickManager ticker(
        // Input
        []() { InputController::poll_events(); },

        // Physics
        [](double delta_time) { world.phys_tick(delta_time); },

        // Graphics
        [&](double delta_time, double subtick_time) {
            world.draw_tick(delta_time, subtick_time);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            world.get_renderer().render(gbuffers);

            poll_gl_errors();

            glfwSwapBuffers(WindowManager::get_active_window());
            
            fps_subtitle.set_value(static_cast<int>(ticker.get_fps()));
            tps_subtitle.set_value(static_cast<int>(ticker.get_real_tps()));
            WindowManager::update_title();
        });

    // Synch physics and graphics ticks, disable TPS requirements
    ticker.set_tps_req(0);

    log_printf(STATUS_REPORTS, "status", "Entering the loop.\n");
    GameLoop::run(ticker, []() {
        return glfwWindowShouldClose(WindowManager::get_active_window());
    });

    poll_gl_errors();

    WindowManager::terminate();

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
