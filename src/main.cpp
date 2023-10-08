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

#include "graphics/gl_debug.h"
#include "graphics/primitives/camera.h"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/texture.h"
#include "io/main_io.h"
#include "logger/debug.h"
#include "logger/logger.h"
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

    GLFWwindow* window = create_window();

    poll_gl_errors();

    Mesh monkey("assets/models/monkey.obj");
    monkey.synch_buffers();
    Camera camera;
    Shader basic_shader("assets/shaders/basic.vsh", "assets/shaders/basic.fsh");
    basic_shader.use();

    Texture texture("assets/textures/rock.jpg");
    texture.bind();

    basic_shader.set_uniform_img("albedo", texture);

    poll_gl_errors();

    camera.set_position(glm::vec3(0.0, 0.0, 3.0));
    camera.direct(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));

    glViewport(0, 0, 800, 600);

    log_printf(STATUS_REPORTS, "status", "Entering main game loop.\n");
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        monkey.render(camera.get_matrix(), basic_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    poll_gl_errors();

    glfwTerminate();

    return errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
