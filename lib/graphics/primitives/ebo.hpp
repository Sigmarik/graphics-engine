/**
 * @file vbo.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief ElementBufferObject
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EBO_HPP
#define EBO_HPP

#include <stdlib.h>

#include "graphics/libs.h"

struct EBO {
    EBO() : id_(0) { glGenBuffers(1, &id_); }
    ~EBO() { glDeleteBuffers(1, &id_); }

    void fill(const unsigned* indices, size_t count) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (GLsizeiptr)(sizeof(*indices) * count), (void*)indices,
                     GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

   private:
    GLuint id_;
};

#endif
