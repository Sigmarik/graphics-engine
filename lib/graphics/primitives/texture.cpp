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

    load_params(data);

    stbi_image_free(data);
}

Texture::Texture(unsigned width, unsigned height, const unsigned char* data,
                 GLint int_format, GLenum format) {
    width_ = width;
    height_ = height;

    int_format_ = int_format;
    format_ = format;

    load_params(data);
}

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0 + slot_);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::attach_to_fb(GLenum attachment) const {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id_, 0);

    poll_gl_errors();
}

void Texture::load_params(const unsigned char* data) const {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, int_format_, (GLsizei)width_,
                 (GLsizei)height_, 0, format_, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    poll_gl_errors();
}
