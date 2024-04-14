#pragma once

#include <vector>

#include "graphics/libs.h"

class WindowManager {
   public:
    static void window_init(size_t width, size_t height,
                                 const char* title = "Unnamed",
                                 bool fullscreen = false);
    
    static GLFWwindow* get_active_window();

   private:
    static GLFWwindow* window_;
};