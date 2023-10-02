/**
 * @file vbo.hpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief VertexBufferObject
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VBO_HPP
#define VBO_HPP

#include "vertex.hpp"

struct VBO {
    VBO() : id_(0) { glGenBuffers(1, &id_); }
    ~VBO() { glDeleteBuffers(1, &id_); }

    void fill(const Vertex* vertices, size_t count) {
        glBufferData(id_, (GLsizeiptr)(sizeof(*vertices) * count),
                     (void*)vertices, GL_STATIC_DRAW);
        count_ = count;
    }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }
    void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

   private:
    GLuint id_;
    size_t count_ = 0;
};

#endif
