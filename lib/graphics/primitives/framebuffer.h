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

struct DepthMap : public FrameBuffer {
    DepthMap(unsigned width, unsigned height);

   private:
    Texture texture_;
};

#endif
