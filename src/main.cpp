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

#include <glm/ext/matrix_transform.hpp>

#include "components/misc/bouncy_head.h"
#include "generation/noise.h"
#include "graphics/gl_debug.h"
#include "graphics/importers/importers.h"
#include "graphics/objects/ambient_light.h"
#include "graphics/objects/decal.h"
#include "graphics/objects/model.h"
#include "graphics/objects/point_light.h"
#include "graphics/objects/postprocessor.h"
#include "graphics/objects/scene.h"
#include "graphics/primitives/camera.h"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/render_frame.h"
#include "graphics/primitives/texture.h"
#include "io/main_io.h"
#include "lib/input/input_controller.h"
#include "logger/debug.h"
#include "logger/logger.h"
#include "logics/scene.h"
#include "managers/asset_manager.h"
#include "managers/world_timer.h"
#include "utils/main_utils.h"

#define MAIN

#include "config.h"

static const unsigned VOLUME_RESOLUTION = 64;

int main(const int argc, char** argv) {
    atexit(log_end_program);

    Options options;

    log_init("program_log.html", LOG_THRESHOLD, &errno);
    print_label();

    log_printf(STATUS_REPORTS, "status", "Initializing\n");

    if (argp_parse(&ARG_P, argc, argv, 0, 0, &options) != 0) {
        return EXIT_FAILURE;
    }

    GLFWwindow* window = create_window();

    poll_gl_errors();

    InputController::init(window);

    Camera camera;

    Scene world = Scene(100.0, 50.0, 1.0);

    BoxCollider ground =
        BoxCollider(Box(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.1, 1.0)),
                    glm::mat4(1.0f));

    world.get_collision().add_collider(ground);

    poll_gl_errors();

    world.get_renderer().set_viewpoint(&camera);

    BouncyHead head(world, glm::vec3(0.0, 20.0, 0.0));

    AmbientLight ambient_light = AmbientLight(glm::vec3(0.3, 0.3, 0.33));
    PointLight point_light = PointLight(glm::vec3(0.7, 0.7, 0.7));
    Postprocessor contrast_vignette =
        Postprocessor(*AssetManager::request<Material>(
            "assets/materials/postprocessing/contrast_vignette.material.xml"));
    Decal splatter =
        Decal(*AssetManager::request<Material>(
                  "assets/materials/decals/red_paint.material.xml"),
              5.0);

    // clang-format off
    point_light.set_object_matrix(glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 2.0, 1.0, 1.0
    ));
    // clang-format on

    // clang-format off
    splatter.set_object_matrix(glm::mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -0.5, 0.5, 0.5, 1.0
    ));
    // clang-format on

    world.get_renderer().track_object(ambient_light);
    world.get_renderer().track_object(point_light);
    world.get_renderer().track_object(contrast_vignette);
    world.get_renderer().track_object(splatter);

    poll_gl_errors();

    camera.set_position(glm::vec3(0.0, 0.0, 3.0));
    camera.direct(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));

    glViewport(0, 0, 800, 600);

    RenderBundle gbuffers(800, 600);

    poll_gl_errors();

    unsigned tick = 0;

    log_printf(STATUS_REPORTS, "status", "Entering the loop.\n");
    while (!glfwWindowShouldClose(window)) {
        tick++;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        head.phys_tick(0.1);
        head.draw_tick(0.1, 0.0);

        world.get_renderer().render(gbuffers);

        poll_gl_errors();

        glfwSwapBuffers(window);

        InputController::poll_events();
    }

    poll_gl_errors();

    glfwTerminate();

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
