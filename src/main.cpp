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

#include "graphics/gl_debug.h"
#include "graphics/importers/importers.h"
#include "graphics/objects/model.h"
#include "graphics/objects/scene.h"
#include "graphics/primitives/camera.h"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/render_frame.h"
#include "graphics/primitives/texture.h"
#include "io/main_io.h"
#include "logger/debug.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"
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

    Mesh monkey("assets/models/monkey.obj");
    monkey.synch_buffers();
    Camera camera;

    RenderManager render_manager(camera);
    poll_gl_errors();

    Model& model =
        *AssetManager::request<Model>("assets/models/monkey.model.xml");

    render_manager.track_object(model);

    poll_gl_errors();

    camera.set_position(glm::vec3(0.0, 0.0, 3.0));
    camera.direct(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));

    glViewport(0, 0, 800, 600);

    RenderBundle gbuffers(800, 600);

    poll_gl_errors();

    unsigned tick = 0;

    glm::mat4 obj_transform = glm::mat4(1.0);

    log_printf(STATUS_REPORTS, "status", "Entering the loop.\n");
    while (!glfwWindowShouldClose(window)) {
        tick++;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        obj_transform =
            glm::rotate(obj_transform, 0.01f, glm::vec3(0.0, 1.0, 0.0));

        model.set_object_matrix(obj_transform);

        render_manager.render(gbuffers);

        poll_gl_errors();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    poll_gl_errors();

    glfwTerminate();

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
