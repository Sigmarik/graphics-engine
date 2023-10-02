/**
 * @file vbo.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief VertexArray
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EBO_HPP
#define EBO_HPP

#include <stdlib.h>

#include "glad/include/glad/gl.h"

struct EBO {
    EBO() : id(0) { glGenBuffers(1, &id); }
    ~EBO() { glDeleteBuffers(1, &id); }

    void fill(const unsigned* indices, size_t count) {
        glBufferData(id, (GLsizeiptr)(sizeof(*indices) * count), (void*)indices,
                     GL_STATIC_DRAW);
    }

    void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
    void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

   private:
    GLuint id;
};

#endif
