#include "render_frame.h"

#include "logger/logger.h"

RenderFrame::RenderFrame(size_t width, size_t height)
    : width_(width), height_(height) {
    glGenFramebuffers(1, &fbo_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);

    init_texture(albedo_tex_id_, GL_COLOR_ATTACHMENT0);
    init_texture(normal_depth_tex_id_, GL_COLOR_ATTACHMENT1);
    init_texture(surface_tex_id_, GL_COLOR_ATTACHMENT2);
    init_texture(final_tex_id_, GL_COLOR_ATTACHMENT3);

    glGenRenderbuffers(1, &rbo_id_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_id_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)width_,
                          (GLsizei)height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo_id_);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log_printf(ERROR_REPORTS, "error", "Framebuffer %u is incomplete\n",
                   fbo_id_);
    }

    const GLenum BUFFERS[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3,
    };
    glDrawBuffers(4, BUFFERS);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderFrame::reset_to_screen() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void RenderFrame::bind_textures(const Shader& shader) const {
    glActiveTexture(GL_TEXTURE0 + GB_SLOT_ALBEDO);
    glBindTexture(GL_TEXTURE_2D, albedo_tex_id_);
    shader.set_uniform_tex_id("GB_COLOR", GB_SLOT_ALBEDO);

    glActiveTexture(GL_TEXTURE0 + GB_SLOT_NORM_DEPTH);
    glBindTexture(GL_TEXTURE_2D, normal_depth_tex_id_);
    shader.set_uniform_tex_id("GB_NORM_DEPTH", GB_SLOT_NORM_DEPTH);

    glActiveTexture(GL_TEXTURE0 + GB_SLOT_SURFACE);
    glBindTexture(GL_TEXTURE_2D, surface_tex_id_);
    shader.set_uniform_tex_id("GB_SURFACE", GB_SLOT_SURFACE);

    glActiveTexture(GL_TEXTURE0 + GB_SLOT_FINAL);
    glBindTexture(GL_TEXTURE_2D, final_tex_id_);
    shader.set_uniform_tex_id("GB_FINAL", GB_SLOT_FINAL);
}

void RenderFrame::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderFrame::init_texture(GLuint& tex_id, GLenum attachment) {
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width_, (GLsizei)height_, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex_id,
                           0);
}

void RenderFrame::use() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_); }

RenderBundle::RenderBundle(unsigned width, unsigned height)
    : front_(width, height), back_(width, height) {}

void RenderBundle::bind_textures(const Shader& shader) const {
    const RenderFrame& in = is_front_ ? front_ : back_;

    in.bind_textures(shader);
}

void RenderBundle::use() const {
    const RenderFrame& out = is_front_ ? back_ : front_;

    out.use();
}

void RenderBundle::swap_frames() { is_front_ = !is_front_; }

void RenderBundle::clear() const {
    front_.clear();
    back_.clear();
}

void RenderBundle::reset_to_screen() { RenderFrame::reset_to_screen(); }
