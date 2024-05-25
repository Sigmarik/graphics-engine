#include "window_manager.h"

#include <sstream>

#include "graphics/gl_debug.h"
#include "logger/logger.h"

GLFWwindow* WindowManager::window_ = nullptr;
std::unordered_map<std::string, std::string> WindowManager::subtitle_info_ = {};
std::string WindowManager::window_title_ = "[UNDEFINED TITLE]";

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

    set_title(title, true);
}

void WindowManager::terminate() { glfwTerminate(); }

GLFWwindow* WindowManager::get_active_window() { return window_; }

void WindowManager::set_subtitle_entry(const std::string& key,
                                       const std::string& value,
                                       bool halt_update) {
    subtitle_info_[key] = value;

    if (!halt_update) update_title();
}

void WindowManager::clear_subtitle(bool halt_update) {
    subtitle_info_.clear();

    if (!halt_update) update_title();
}

void WindowManager::remove_subtitle_entry(const std::string& key,
                                          bool halt_update) {
    subtitle_info_.erase(key);

    if (!halt_update) update_title();
}

void WindowManager::set_title(const char* title, bool halt_update) {
    window_title_ = title;

    if (!halt_update) update_title();
}

void WindowManager::update_title() {
    std::stringstream stream;

    stream << window_title_;

    if (!subtitle_info_.empty()) {
        stream << " (";

        size_t index = 0;
        for (const auto& [key, value] : subtitle_info_) {
            stream << key << ": " << value;

            if (index + 1 != subtitle_info_.size()) stream << ", ";

            ++index;
        }

        stream << ")";
    }

    glfwSetWindowTitle(window_, stream.str().c_str());
}
