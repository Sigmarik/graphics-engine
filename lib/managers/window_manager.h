#pragma once

#include <concepts>
#include <ctime>
#include <string>
#include <unordered_map>

#include "graphics/libs.h"

struct WindowManager final {
    static void init(size_t width, size_t height, const char* title = "Unnamed",
                     bool fullscreen = false);

    static void terminate();

    static GLFWwindow* get_active_window();

    static bool valid() { return window_ != nullptr; }

    struct SubtitleEntry final {
        template <class ValueType>
        void set_value(const ValueType& value) {
            value_ = std::to_string(value);
        }

        std::string get_value() const { return value_; }

       private:
        std::string value_ = "";
    };

    /**
     * @brief Add window subtitle parameter.
     *
     * @param[in] name subtitle parameter name (f.e. "FPS")
     */
    static SubtitleEntry& add_subtitle_param(const std::string& name);

    /**
     * @brief Remove all subtitle entries.
     *
     * @param[in] halt_update
     */
    static void clear_subtitle(bool halt_update = false);

    /**
     * @brief Remove one specific subtitle info entry.
     *
     * @param[in] key
     * @param[in] halt_update
     */
    static void remove_subtitle_entry(const std::string& key,
                                      bool halt_update = false);

    /**
     * @brief Set window title.
     *
     * @warning Should be preferred over `glfwSetWindowTitle`, as this function
     * respects window subtitles
     *
     * @param[in] halt_update
     */
    static void set_title(const char* title, bool halt_update = false);

    /**
     * @brief Set title update frequency
     *
     * @param[in] freq frequency in Hz
     */
    static void set_title_update_frequency(size_t freq);

    static void update_title();

   private:
    static size_t title_update_frequency_;
    static double last_update_time_in_sec_;

    static GLFWwindow* window_;
    static std::unordered_map<std::string, SubtitleEntry> subtitle_info_;
    static std::string window_title_;
};