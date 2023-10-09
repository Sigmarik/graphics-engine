#include "framebuffer.h"

RenderBuffer::RenderBuffer(unsigned width, unsigned height, GLenum storage)
    : width_(width), height_(height) {
    glGenRenderbuffers(1, &id_);

    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, storage, (GLsizei)width_,
                          (GLsizei)height_);
}

void RenderBuffer::bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id_); }

void RenderBuffer::attach_to_fb(GLenum attachment) const {
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, id_);
}

FrameBuffer::FrameBuffer() { glGenFramebuffers(1, &id_); }

FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &id_); }

void FrameBuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id_); }

void FrameBuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

RenderTarget::RenderTarget(unsigned width, unsigned height)
    : FrameBuffer(),
      texture_(width, height, 0),
      rbo_(width, height, GL_DEPTH24_STENCIL8) {
    texture_.attach_to_fb(GL_COLOR_ATTACHMENT0);
    rbo_.attach_to_fb(GL_DEPTH_STENCIL_ATTACHMENT);
}

const Texture& RenderTarget::get_texture() const { return texture_; }

DepthMap::DepthMap(unsigned width, unsigned height)
    : FrameBuffer(),
      texture_(width, height, 0, nullptr, GL_DEPTH24_STENCIL8,
               GL_UNSIGNED_INT_24_8) {
    texture_.attach_to_fb(GL_DEPTH_STENCIL_ATTACHMENT);
}
