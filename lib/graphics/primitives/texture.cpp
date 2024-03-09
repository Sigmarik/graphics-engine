#include "texture.h"

#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "graphics/gl_debug.h"
#include "logger/logger.h"

Texture::Texture(const char* path) {
    int chanel_count = 0;
    int width = 0, height = 0;
    unsigned char* data = stbi_load(path, &width, &height, &chanel_count, 0);

    if (!data) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load image form file %s\n", path);
        return;
    }

    if (chanel_count != 3) {
        log_printf(ERROR_REPORTS, "error",
                   "Image %s contains %d channels. 3 expected.\n", path,
                   chanel_count);
        return;
    }

    width_ = (unsigned)width;
    height_ = (unsigned)height;

    glGenTextures(1, &id_);

    load_params(data);

    stbi_image_free(data);
}

Texture::Texture(unsigned width, unsigned height, const unsigned char* data,
                 GLint int_format, GLenum format) {
    width_ = width;
    height_ = height;

    int_format_ = int_format;
    format_ = format;

    glGenTextures(1, &id_);

    load_params(data);

    poll_gl_errors();
}

void Texture::bind(unsigned slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id_);

    poll_gl_errors();
}

void Texture::attach_to_fb(GLenum attachment) const {
    poll_gl_errors();
    bind(0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id_, 0);

    poll_gl_errors();
}

void Texture::use_settings(const TextureSettings& settings) const {
    poll_gl_errors();
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.interp);
    poll_gl_errors();
}

void Texture::load_params(const unsigned char* data) const {
    poll_gl_errors();

    // Silent `bind(0)`
    use_settings({.wrap = GL_REPEAT, .interp = GL_NEAREST});

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, int_format_, (GLsizei)width_,
                 (GLsizei)height_, 0, format_, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    poll_gl_errors();
}

//* 3-DIMENSIONAL TEXTURE

Texture3D::Texture3D(unsigned size_x, unsigned size_y, unsigned size_z,
                     GLint int_format, GLenum format) {
    size_x_ = size_x;
    size_y_ = size_y;
    size_z_ = size_z;
    int_format_ = int_format;
    format_ = format;

    glGenTextures(1, &id_);

    load_params(nullptr);

    poll_gl_errors();
}

void Texture3D::bind(unsigned slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_3D, id_);

    poll_gl_errors();
}

void Texture3D::synch(const unsigned char* data) const {
    glTexImage3D(GL_TEXTURE_3D, 0, int_format_, (GLsizei)size_x_,
                 (GLsizei)size_y_, (GLsizei)size_z_, 0, format_,
                 GL_UNSIGNED_BYTE, data);

    poll_gl_errors();
}

void Texture3D::use_settings(const TextureSettings& settings) const {
    poll_gl_errors();
    bind(0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, settings.wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, settings.wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, settings.wrap);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, settings.interp);
    poll_gl_errors();
}

void Texture3D::load_params(const unsigned char* data) const {
    poll_gl_errors();

    // Silent `bind(0)`
    use_settings({.wrap = GL_REPEAT, .interp = GL_LINEAR});

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    synch(data);
    glGenerateMipmap(GL_TEXTURE_3D);

    poll_gl_errors();
}
