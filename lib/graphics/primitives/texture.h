/**
 * @file texture.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief 2D texture
 * @version 0.1
 * @date 2023-10-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "graphics/libs.h"

struct Texture {
    Texture(const char* path);
    Texture(unsigned width, unsigned height,
            const unsigned char* data = nullptr, GLint int_format = GL_RGB,
            GLenum format = GL_RGB);

    void set_slot(unsigned slot) { slot_ = slot; }
    unsigned get_slot() const { return slot_; }

    void bind() const;

    void attach_to_fb(GLenum attachment) const;

   protected:
    void load_params(const unsigned char* data = nullptr) const;

   private:
    Texture& operator=(const Texture& texture) = default;
    Texture(const Texture& texture) = default;

    GLint int_format_ = GL_RGB;
    GLenum format_ = GL_RGB;
    GLuint id_ = 0;
    unsigned slot_ = 0;
    unsigned width_ = 0, height_ = 0;
};

#endif
