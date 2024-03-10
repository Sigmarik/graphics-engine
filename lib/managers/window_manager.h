#pragma once

#include <vector>

#include "graphics/libs.h"

typedef size_t WindowID;

class WindowManager {
   public:
    static GLFWwindow* get_active_window();

    static void set_active_window(WindowID active_window_id);

    static WindowID construct_window(size_t width, size_t height,
                                     bool fullscreen);

    static const WindowID Invalid_window_id = ~WindowID(0);

   private:
    static std::vector<GLFWwindow*> windows_;
    static WindowID active_window_id_;
};