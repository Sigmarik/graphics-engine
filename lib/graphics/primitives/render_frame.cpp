#include "render_frame.h"

#include "logger/logger.h"

RenderFrame::RenderFrame(size_t width, size_t height)
    : width_(width), height_(height) {
    glGenFramebuffers(1, &fbo_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);

    glGenTextures(1, &albedo_tex_id_);
    glBindTexture(GL_TEXTURE_2D, albedo_tex_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width_, (GLsizei)height_, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           albedo_tex_id_, 0);

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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderFrame::reset_to_screen() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void RenderFrame::bind_textures(const Shader& shader) const {
    glActiveTexture(GL_TEXTURE0 + 10);
    glBindTexture(GL_TEXTURE_2D, albedo_tex_id_);
    log_printf(STATUS_REPORTS, "status", "Currently used albedo texture: %u\n",
               albedo_tex_id_);
    shader.set_uniform_tex_id("GB_COLOR", 10);
}

void RenderFrame::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderFrame::use() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
    // log_printf(STATUS_REPORTS, "status", "Framebuffer id: %u\n", fbo_id_);
    log_printf(STATUS_REPORTS, "status",
               "Currently used output albedo texture: %u\n", albedo_tex_id_);
}

RenderBundle::RenderBundle(unsigned width, unsigned height)
    : front_(width, height), back_(width, height) {}

void RenderBundle::bind_textures(const Shader& shader) const {
    const RenderFrame& in = is_front_ ? front_ : back_;

    log_printf(STATUS_REPORTS, "status", "Binding textures from frame %d\n",
               is_front_);

    in.bind_textures(shader);
}

void RenderBundle::use() const {
    const RenderFrame& out = is_front_ ? back_ : front_;

    log_printf(STATUS_REPORTS, "status", "Binding frame %d as output\n",
               !is_front_);

    out.use();
}

void RenderBundle::swap_frames() { is_front_ = !is_front_; }

void RenderBundle::clear() const {
    front_.clear();
    back_.clear();
}

void RenderBundle::reset_to_screen() { RenderFrame::reset_to_screen(); }
