#include "framebuffer.h"

#include "graphics/gl_debug.h"
#include "logger/logger.h"
#include "shader.h"

RenderBuffer::RenderBuffer(unsigned width, unsigned height, GLenum storage)
    : width_(width), height_(height) {
    glGenRenderbuffers(1, &id_);

    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, storage, (GLsizei)width_,
                          (GLsizei)height_);

    poll_gl_errors();
}

void RenderBuffer::bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id_); }

void RenderBuffer::attach_to_fb(GLenum attachment) const {
    poll_gl_errors();
    // bind();
    poll_gl_errors();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id_);
    // unbind();
    poll_gl_errors();
}

FrameBuffer::FrameBuffer() {
    poll_gl_errors();
    glGenFramebuffers(1, &id_);
    log_printf(STATUS_REPORTS, "status", "Generated framebuffer with id %u\n",
               id_);
    poll_gl_errors();
}

FrameBuffer::~FrameBuffer() {
    poll_gl_errors();
    glDeleteFramebuffers(1, &id_);
    poll_gl_errors();
}

void FrameBuffer::bind() const {
    poll_gl_errors();
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    poll_gl_errors();
}

void FrameBuffer::unbind() const {
    poll_gl_errors();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    poll_gl_errors();
}

RenderTarget::RenderTarget(unsigned width, unsigned height)
    : FrameBuffer(),
      texture_(width, height, 0),
      rbo_(width, height, GL_DEPTH24_STENCIL8) {
    bind();
    texture_.attach_to_fb(GL_COLOR_ATTACHMENT0);
    rbo_.attach_to_fb(GL_DEPTH_STENCIL_ATTACHMENT);
    unbind();
    poll_gl_errors();
}

const Texture& RenderTarget::get_texture() const { return texture_; }

DepthMap::DepthMap(unsigned width, unsigned height)
    : FrameBuffer(),
      texture_(width, height, 0, nullptr, GL_DEPTH24_STENCIL8,
               GL_UNSIGNED_INT_24_8) {
    bind();
    texture_.attach_to_fb(GL_DEPTH_STENCIL_ATTACHMENT);
    unbind();
    poll_gl_errors();
}
