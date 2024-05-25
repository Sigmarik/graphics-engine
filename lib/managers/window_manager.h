#pragma once

#include <string>
#include <unordered_map>

#include "graphics/libs.h"

class WindowManager {
   public:
    static void init(size_t width, size_t height, const char* title = "Unnamed",
                     bool fullscreen = false);

    static void terminate();

    static GLFWwindow* get_active_window();

    static bool valid() { return window_ != nullptr; }

    /**
     * @brief Set window subtitle entry.
     *
     * @param[in] key subtitle key (f.e. "FPS")
     * @param[in] value value of the entry (f.e. "60")
     * @param[in] halt_update
     */
    static void set_subtitle_entry(const std::string& key,
                                   const std::string& value,
                                   bool halt_update = false);

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

   private:
    static void update_title();

    static GLFWwindow* window_;
    static std::unordered_map<std::string, std::string> subtitle_info_;
    static std::string window_title_;
};