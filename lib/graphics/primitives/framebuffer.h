/**
 * @file framebuffer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Different render targets
 * @version 0.1
 * @date 2023-10-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "graphics/libs.h"
#include "shader.h"
#include "texture.h"

struct RenderBuffer {
    RenderBuffer(unsigned width, unsigned height,
                 GLenum storage = GL_DEPTH24_STENCIL8);

    void bind() const;

    void attach_to_fb(GLenum attachment) const;

   private:
    GLuint id_ = 0;
    unsigned width_, height_;
};

struct FrameBuffer {
    FrameBuffer();
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

   private:
    GLuint id_ = 0;
};

struct RenderTarget : public FrameBuffer {
    RenderTarget(unsigned width, unsigned height);

    const Texture& get_texture() const;

   private:
    Texture texture_;
    RenderBuffer rbo_;
};

struct RenderBundle : public FrameBuffer {
    RenderBundle(unsigned width, unsigned height);

    const Texture& color_tx() const { return color_; }
    const Texture& normal_tx() const { return normal_; }
    const Texture& light_params_tx() const { return surface_; }
    const Texture& light_tx() const { return light_; }

    void bind_as_input(const Shader& shader) const;

    void clear() const;

   private:
    Texture color_;
    Texture normal_;
    Texture surface_;
    Texture light_;
    RenderBuffer depth_stencil_;
};

struct DepthMap : public FrameBuffer {
    DepthMap(unsigned width, unsigned height);

   private:
    Texture texture_;
};

#endif
