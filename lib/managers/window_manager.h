#pragma once

#include <vector>

#include "graphics/libs.h"

class WindowManager {
   public:
    static void init(size_t width, size_t height, const char* title = "Unnamed",
                     bool fullscreen = false);

    static void terminate();

    static GLFWwindow* get_active_window();

    static bool valid() { return window_ != nullptr; }

   private:
    static GLFWwindow* window_;
};