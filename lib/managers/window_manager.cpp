#include "window_manager.h"

#include <sstream>

#include "graphics/gl_debug.h"
#include "logger/logger.h"
#include "managers/world_timer.h"

GLFWwindow* WindowManager::window_ = nullptr;
std::vector<std::weak_ptr<WindowManager::SubtitleDataBlock>>
    WindowManager::subtitle_info_{};
std::string WindowManager::window_title_ = "[UNDEFINED TITLE]";
double WindowManager::last_title_update_ = 0.0;
double WindowManager::subtitle_update_dt_ = 0.0;
bool WindowManager::requires_title_update_ = false;

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

void WindowManager::terminate() {
    glfwTerminate();
    window_ = nullptr;
}

GLFWwindow* WindowManager::get_active_window() { return window_; }

void WindowManager::set_title(const char* title, bool halt_update) {
    window_title_ = title;

    request_title_update();

    if (!halt_update) update_title();
}

void WindowManager::update_title() {
    if (!valid()) return;
    if (!requires_title_update_) return;

    last_title_update_ = WorldTimer::get_time_sec();
    requires_title_update_ = false;

    std::stringstream stream;

    stream << window_title_;

    if (!subtitle_info_.empty()) {
        stream << " (";

        size_t index = 0;
        for (const auto& entry : subtitle_info_) {
            SubtitleDataBlock& data = *entry.lock();
            stream << data.key << ": " << data.value;

            if (index + 1 != subtitle_info_.size()) stream << ", ";

            ++index;
        }

        stream << ")";
    }

    glfwSetWindowTitle(window_, stream.str().c_str());
}

void WindowManager::refresh() {
    if (!valid()) return;

    double time = WorldTimer::get_time_sec();

    if (last_title_update_ + subtitle_update_dt_ < time) {
        update_title();
    }

    glfwSwapBuffers(get_active_window());
}

WindowManager::SubtitleEntry WindowManager::add_subtitle_entry(
    const std::string& key, double frequency) {
    double delta_time = frequency == 0.0 ? 0.0 : 1.0 / frequency;
    SubtitleEntry entry(key, delta_time);

    subtitle_info_.push_back(entry.get_data());

    subtitle_update_dt_ = std::min(subtitle_update_dt_, delta_time);

    return entry;
}

void WindowManager::notify_subtitle_stat_change() {
    if (subtitle_info_.empty()) return;

    size_t left_id = 0;

    for (size_t right_id = 0; right_id < subtitle_info_.size(); ++right_id) {
        if (!subtitle_info_[right_id].expired()) {
            subtitle_info_[left_id] = subtitle_info_[right_id];
            ++left_id;
        }
    }

    subtitle_info_.resize(left_id);

    if (subtitle_info_.empty()) return;

    subtitle_update_dt_ = subtitle_info_[0].lock()->refresh_dt;

    for (const auto& entry : subtitle_info_) {
        subtitle_update_dt_ =
            std::min(entry.lock()->refresh_dt, subtitle_update_dt_);
    }
}

void WindowManager::SubtitleEntry::hide() {
    if (!*this) return;

    data_.reset();
    WindowManager::notify_subtitle_stat_change();
}

WindowManager::SubtitleEntry::~SubtitleEntry() { hide(); }

WindowManager::SubtitleEntry::SubtitleEntry(const std::string& key,
                                            double refresh_dt)
    : data_(std::make_shared<WindowManager::SubtitleDataBlock>()) {
    data_->key = key;
    data_->refresh_dt = refresh_dt;
}
