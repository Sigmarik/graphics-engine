#pragma once

#include <memory>
#include <string>
#include <vector>

#include "graphics/libs.h"

struct WindowManager {
    /**
     * @brief Construct the main program window and initialize graphics
     * libraries.
     *
     * @param[in] width window width
     * @param[in] height window height
     * @param[in] title window title ("Unnamed" by default)
     * @param[in] fullscreen weather the window should be in full screen mode
     *
     * @note In case of an error the window pointer remains `nullptr`, which can
     * be checked using `WindowManager::valid()` call.
     */
    static void init(size_t width, size_t height, const char* title = "Unnamed",
                     bool fullscreen = false);

    /**
     * @brief Destroy the main window
     *
     */
    static void terminate();

    /**
     * @brief Get the currently active window
     *
     * @return GLFWwindow*
     */
    static GLFWwindow* get_active_window();

    /**
     * @brief Check weather the window has been initialized
     *
     * @return true
     * @return false
     */
    static bool valid() { return window_ != nullptr; }

    struct SubtitleEntry;
    friend SubtitleEntry;

    /**
     * @brief Set window title.
     *
     * @warning Should be preferred over `glfwSetWindowTitle`, as this function
     * respects window subtitles
     *
     * @param[in] halt_update weather the title update should be halted
     */
    static void set_title(const char* title, bool halt_update = false);

    /**
     * @brief Force-update the title
     *
     */
    static void update_title();

    /**
     * @brief Swap display buffers and update window title if needed.
     *
     */
    static void refresh();

    struct SubtitleDataBlock {
        std::string key = "UNDEF_KEY";
        std::string value = "UNDEFINED";
        double refresh_dt = 0.1;
    };

    /**
     * @brief Add an entry to the window subtitle `window_title (subtitle_key:
     * value, key: value)`.
     *
     * @param[in] key entry key
     * @param[in] frequency subtitle update frequency requirement (zero causes
     * the title to update as frequently as possible) (10 times per second by
     * default)
     * @return SubtitleEntry
     */
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
    /**
     * @brief Set displayed value of the entry
     *
     * @param[in] value
     *
     * @note Call after `hide` will have no effect on the title.
     */
    void set_value(const std::string& value) {
        if (!*this) return;

        data_->value = value;
        WindowManager::request_title_update();
    }

    /**
     * @brief Set displayed value of the entry
     *
     * @tparam T argument type (should be convertible to string using
     * std::to_string)
     * @param[in] value
     *
     * @note Call after `hide` will have no effect on the title.
     */
    template <class T>
    requires requires(T&& t) { std::to_string(t); }
    void set_value(T&& value) {
        if (!*this) return;

        data_->value = std::to_string(value);
        WindowManager::request_title_update();
    }

    friend WindowManager;

    /**
     * @brief Remove entry from the window subtitle.
     *
     * @note Can be called however many times over.
     * @note Does not affect correctness of `set_value` calls.
     */
    void hide();

    operator bool() const { return static_cast<bool>(data_); }

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
