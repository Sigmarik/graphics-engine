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

struct TextureSettings {
    int wrap = GL_REPEAT;
    int interp = GL_NEAREST;
};

struct Texture {
    Texture(const char* path);
    Texture(unsigned width, unsigned height,
            const unsigned char* data = nullptr, GLint int_format = GL_RGB,
            GLenum format = GL_RGB);

    Texture& operator=(const Texture& texture) = delete;
    Texture(const Texture& texture) = delete;

    void bind(unsigned slot) const;

    void attach_to_fb(GLenum attachment) const;

    void use_settings(const TextureSettings& settings) const;

   protected:
    void load_params(const unsigned char* data = nullptr) const;

   private:
    GLint int_format_ = GL_RGB;
    GLenum format_ = GL_RGB;
    GLuint id_ = 0;
    unsigned width_ = 0, height_ = 0;
};

struct Texture3D {
    Texture3D(unsigned size_x, unsigned size_y, unsigned size_z,
              GLint int_format = GL_RGB, GLenum format = GL_RGB);

    Texture3D& operator=(const Texture3D& texture) = delete;
    Texture3D(const Texture3D& texture) = delete;

    void bind(unsigned slot) const;

    void synch(const unsigned char* data = nullptr) const;

    void use_settings(const TextureSettings& settings) const;

   protected:
    void load_params(const unsigned char* data = nullptr) const;

   private:
    GLint int_format_ = GL_RGB;
    GLenum format_ = GL_RGB;
    GLuint id_ = 0;
    unsigned size_x_ = 0, size_y_ = 0, size_z_ = 0;
};

#endif
