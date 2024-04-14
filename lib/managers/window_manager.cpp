#include "window_manager.h"

#include "lib/logger/logger.h"

GLFWwindow* WindowManager::window_ = nullptr;

void WindowManager::window_init(size_t width, size_t height,
                                     const char* title, bool fullscreen) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    window_ =
        glfwCreateWindow(int(width), int(height), title,
                         fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    if (window_ == NULL) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to initialize window. Terminating.\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window_);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to initialize GLAD. Terminating.\n");
        return;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
}

GLFWwindow* WindowManager::get_active_window() {
    return window_;
}
