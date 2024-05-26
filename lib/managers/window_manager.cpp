#include "window_manager.h"

#include <sstream>

#include "graphics/gl_debug.h"
#include "logger/logger.h"
#include "managers/world_timer.h"

GLFWwindow* WindowManager::window_ = nullptr;
std::unordered_map<std::string, WindowManager::SubtitleEntry>
    WindowManager::subtitle_info_ = {};
std::string WindowManager::window_title_ = "[UNDEFINED TITLE]";
size_t WindowManager::title_update_frequency_ = 60;
double WindowManager::last_update_time_in_sec_ = WorldTimer::get_time_sec();

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

WindowManager::SubtitleEntry& WindowManager::add_subtitle_param(
    const std::string& name) {
    return subtitle_info_[name] = SubtitleEntry();
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

void WindowManager::set_title_update_frequency(size_t freq) {
    title_update_frequency_ = freq;
}

void WindowManager::update_title() {
    auto new_time = WorldTimer::get_time_sec();
    auto delta_time = new_time - last_update_time_in_sec_;
    auto expected_delta_time = 1.0 / static_cast<double>(title_update_frequency_);
    last_update_time_in_sec_ = new_time;
    if (title_update_frequency_ > 0)
        if (delta_time < expected_delta_time) 
            return;

    std::stringstream stream;

    stream << window_title_;

    if (!subtitle_info_.empty()) {
        stream << " (";

        size_t index = 0;
        for (const auto& [key, value] : subtitle_info_) {
            stream << key << ": " << value.get_value();

            if (index + 1 != subtitle_info_.size()) stream << ", ";

            ++index;
        }

        stream << ")";
    }

    glfwSetWindowTitle(window_, stream.str().c_str());
}
