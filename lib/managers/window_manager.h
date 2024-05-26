#pragma once

#include <memory>
#include <string>
#include <vector>

#include "graphics/libs.h"

struct WindowManager {
    static void init(size_t width, size_t height, const char* title = "Unnamed",
                     bool fullscreen = false);

    static void terminate();

    static GLFWwindow* get_active_window();

    static bool valid() { return window_ != nullptr; }

    struct SubtitleEntry;
    friend SubtitleEntry;

    /**
     * @brief Set window title.
     *
     * @warning Should be preferred over `glfwSetWindowTitle`, as this function
     * respects window subtitles
     *
     * @param[in] halt_update
     */
    static void set_title(const char* title, bool halt_update = false);

    static void update_title();

    static void refresh();

    struct SubtitleDataBlock {
        std::string key = "UNDEF_KEY";
        std::string value = "UNDEFINED";
        double refresh_dt = 0.1;
    };

    static SubtitleEntry add_subtitle_entry(const std::string& key,
                                            double frequency = 10.0);

   private:
    static void notify_subtitle_stat_change();

    static void request_title_update() { requires_title_update_ = true; }

    static GLFWwindow* window_;
    static std::vector<std::weak_ptr<SubtitleDataBlock>> subtitle_info_;
    static std::string window_title_;

    static double last_title_update_;
    static double subtitle_update_dt_;

    static bool requires_title_update_;
};

struct WindowManager::SubtitleEntry {
    void set_value(const std::string& value) {
        data_->value = value;
        WindowManager::request_title_update();
    }

    template <class T>
    requires requires(T&& t) { std::to_string(t); }
    void set_value(T&& value) {
        data_->value = std::to_string(value);
        WindowManager::request_title_update();
    }

    friend WindowManager;

    ~SubtitleEntry();

   private:
    SubtitleEntry(const std::string& key, double refresh_dt);

    SubtitleEntry(const SubtitleEntry& entry) = default;
    SubtitleEntry(SubtitleEntry&& entry) = default;
    SubtitleEntry& operator=(const SubtitleEntry& entry) = default;
    SubtitleEntry& operator=(SubtitleEntry&& entry) = default;

    std::weak_ptr<WindowManager::SubtitleDataBlock> get_data() { return data_; }

    std::shared_ptr<WindowManager::SubtitleDataBlock> data_;
};
