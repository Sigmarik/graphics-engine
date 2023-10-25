#include "main_utils.h"

#include <stdlib.h>

#include "graphics/libs.h"
#include "lib/logger/logger.h"
#include "src/config.h"

void print_label() {
    puts(PROGRAM_NAME);
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n",
               __DATE__, __TIME__);
}

GLFWwindow* create_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Window Title", NULL, NULL);
    if (window == NULL) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to initialize window. Terminating.\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to initialize GLAD. Terminating.\n");
        return nullptr;
    }
    glEnable(GL_MULTISAMPLE);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    return window;
}
