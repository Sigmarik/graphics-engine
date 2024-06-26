/**
 * @file mesh.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief 3D model asset
 * @version 0.1
 * @date 2023-09-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

#include "ebo.hpp"
#include "shader.h"
#include "vao.hpp"
#include "vbo.hpp"
#include "vertex.hpp"

struct aiMesh;

struct Mesh final {
    Mesh() = default;
    explicit Mesh(const char* path);
    explicit Mesh(const aiMesh& mesh);

    Mesh(const Mesh&) = default;

    void load(const char* path);
    void append(const std::vector<Vertex>& vertices,
                const std::vector<unsigned>& indices, const glm::mat4 matrix);

    void synch_buffers();

    void render(const glm::mat4& proj_matrix, const glm::mat4& obj_matrix,
                const Shader& shader) const;

    static Mesh parse_ai_mesh(const aiMesh& mesh);

   private:
    std::vector<Vertex> vertices_ = {};
    std::vector<unsigned> indices_ = {};
    VAO vao_ = VAO();
    VBO vbo_ = VBO();
    EBO ebo_ = EBO();
};
