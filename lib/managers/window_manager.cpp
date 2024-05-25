#include "window_manager.h"

#include "graphics/gl_debug.h"
#include "logger/logger.h"

GLFWwindow* WindowManager::window_ = nullptr;

void WindowManager::init(size_t width, size_t height, const char* title,
                         bool fullscreen) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window =
        glfwCreateWindow(int(width), int(height), title,
                         fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    if (window == NULL) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to initialize the window.\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        log_printf(ERROR_REPORTS, "error", "Failed to initialize GLAD.\n");
        return;
    }

    window_ = window;

    glClearColor(1.f, 0.f, 1.f, 0.0f);

    poll_gl_errors();
}

void WindowManager::terminate() { glfwTerminate(); }

GLFWwindow* WindowManager::get_active_window() { return window_; }
