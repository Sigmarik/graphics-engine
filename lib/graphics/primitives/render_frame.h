/**
 * @file render_frame.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Render frame class
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RENDER_FRAME_H
#define RENDER_FRAME_H

#include <stdlib.h>

#include "graphics/libs.h"
#include "shader.h"

struct RenderFrame {
    RenderFrame(size_t width, size_t height);

    void use() const;
    static void reset_to_screen();
    void bind_textures(const Shader& shader) const;

    void clear() const;

    size_t get_width() const { return width_; }
    size_t get_height() const { return height_; }

   private:
    size_t width_, height_;
    GLuint fbo_id_ = 0, albedo_tex_id_ = 0, rbo_id_ = 0;
};

#endif
