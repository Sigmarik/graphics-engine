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

RenderBundle::RenderBundle(unsigned width, unsigned height)
    : FrameBuffer(),
      color_(width, height, 11, nullptr, GL_RGBA, GL_RGBA),
      normal_(width, height, 10, nullptr),
      surface_(width, height, 9, nullptr),
      light_(width, height, 8, nullptr),
      depth_stencil_(width, height, GL_DEPTH_COMPONENT24) {
    poll_gl_errors();
    bind();
    color_.attach_to_fb(GL_COLOR_ATTACHMENT0);
    // normal_.attach_to_fb(GL_COLOR_ATTACHMENT1);
    // surface_.attach_to_fb(GL_COLOR_ATTACHMENT2);
    // light_.attach_to_fb(GL_COLOR_ATTACHMENT3);
    depth_stencil_.attach_to_fb(GL_DEPTH_ATTACHMENT);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    clear();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log_printf(ERROR_REPORTS, "error",
                   "Render bundle incomplete after creation\n");
    }

    unbind();

    poll_gl_errors();
}

void RenderBundle::bind_as_input(const Shader& shader) const {
    poll_gl_errors();
    shader.use();
    shader.set_uniform_tex("GB_COLOR", color_);
    shader.set_uniform_tex("GB_NORMAL", normal_);
    shader.set_uniform_tex("GB_SURFACE", surface_);
    shader.set_uniform_tex("GB_LIGHT", light_);
    poll_gl_errors();
}

void RenderBundle::clear() const {
    poll_gl_errors();
    bind();
    // const GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0,
    // GL_COLOR_ATTACHMENT1,
    //                                GL_COLOR_ATTACHMENT2,
    //                                GL_COLOR_ATTACHMENT3};
    // glDrawBuffers(4, draw_buffers);
    const GLuint CLEAR_COLOR[4] = {255, 0, 255, 255};
    glClearBufferuiv(GL_COLOR, 0, CLEAR_COLOR);
    glClear(GL_COLOR_BUFFER_BIT);
    unbind();
    poll_gl_errors();
}
