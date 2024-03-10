#include "window_manager.h"

#include "lib/logger/logger.h"

std::vector<GLFWwindow*> WindowManager::windows_ = {};
WindowID WindowManager::active_window_id_ = 0;

GLFWwindow* WindowManager::get_active_window() {
    return windows_.at(active_window_id_);
}

void WindowManager::set_active_window(WindowID active_window_id) {
    active_window_id_ = active_window_id;
}

WindowID WindowManager::construct_window(size_t width, size_t height,
                                         bool fullscreen) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window =
        glfwCreateWindow(int(width), int(height), "Window Title",
                         fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (window == NULL) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to initialize window. Terminating.\n");
        glfwTerminate();
        return Invalid_window_id;
    }

    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to initialize GLAD. Terminating.\n");
        return Invalid_window_id;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    windows_.push_back(window);
    return windows_.size() - 1;
}