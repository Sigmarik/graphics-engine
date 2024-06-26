#include "mesh.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdio.h>

#include <assimp/Importer.hpp>
#include <string>

#include "graphics/gl_debug.h"
#include "logger/logger.h"

Mesh::Mesh(const char* path) { load(path); }

Mesh::Mesh(const aiMesh& mesh) {
    size_t vertex_count = mesh.mNumVertices;

    unsigned uv_depth = mesh.GetNumUVChannels();

    for (size_t vrt_id = 0; vrt_id < vertex_count; ++vrt_id) {
        aiVector3D pos = mesh.mVertices[vrt_id];
        aiVector3D normal = mesh.mNormals[vrt_id];
        aiVector3D uv = (uv_depth > 0 && mesh.mTextureCoords[0] != nullptr)
                            ? mesh.mTextureCoords[0][vrt_id]
                            : aiVector3D(0.0, 0.0, 0.0);
        aiVector3D tangent = mesh.mTangents[vrt_id];
        vertices_.push_back(
            (Vertex){.position = glm::vec3(pos.x, pos.y, pos.z),
                     .normal = glm::vec3(normal.x, normal.y, normal.z),
                     .uv = glm::vec2(uv.x, uv.y),
                     .tangent = glm::vec3(tangent.x, tangent.y, tangent.z)});
    }

    for (size_t face_id = 0; face_id < mesh.mNumFaces; ++face_id) {
        aiFace face = mesh.mFaces[face_id];

        for (unsigned id = 0; id < 3; ++id) {
            if (face.mIndices[id] >= mesh.mNumVertices) {
                log_printf(ERROR_REPORTS, "error",
                           "Face vertex index overflow (requested vertex %u "
                           "out of %u)\n",
                           face.mIndices[id], mesh.mNumVertices);
            }

            indices_.push_back(face.mIndices[id]);
        }
    }

    synch_buffers();
}

void Mesh::load(const char* path) {
    static Assimp::Importer import;

    poll_gl_errors();

    log_printf(STATUS_REPORTS, "status", "Loading model %s\n", path);

    const aiScene* scene = import.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs |
                  aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        log_printf(ERROR_REPORTS, "error", "Failed to load model %s\n", path);
        return;
    }

    aiMesh** meshes = scene->mMeshes;

    size_t index_shift = 0;

    for (size_t mesh_id = 0; mesh_id < scene->mNumMeshes; ++mesh_id) {
        aiMesh* mesh = meshes[mesh_id];

        //* `_box_` objects define collision boxes and should not be imported
        if (strncmp(mesh->mName.C_Str(), "_box_", 5) == 0) continue;

        size_t vertex_count = mesh->mNumVertices;

        unsigned uv_depth = mesh->GetNumUVChannels();

        for (size_t vrt_id = 0; vrt_id < vertex_count; ++vrt_id) {
            aiVector3D pos = mesh->mVertices[vrt_id];
            aiVector3D normal = mesh->mNormals[vrt_id];
            aiVector3D uv = (uv_depth > 0 && mesh->mTextureCoords[0] != nullptr)
                                ? mesh->mTextureCoords[0][vrt_id]
                                : aiVector3D(0.0, 0.0, 0.0);
            aiVector3D tangent = mesh->mTangents[vrt_id];
            vertices_.push_back((Vertex){
                .position = glm::vec3(pos.x, pos.y, pos.z),
                .normal = glm::vec3(normal.x, normal.y, normal.z),
                .uv = glm::vec2(uv.x, uv.y),
                .tangent = glm::vec3(tangent.x, tangent.y, tangent.z)});
        }

        for (size_t face_id = 0; face_id < mesh->mNumFaces; ++face_id) {
            aiFace face = mesh->mFaces[face_id];
            for (unsigned id = 0; id < 3; ++id) {
                indices_.push_back(face.mIndices[id] + (unsigned)index_shift);
            }
        }

        index_shift = vertices_.size();
    }

    log_printf(STATUS_REPORTS, "status",
               "Loading successful, read %lu vertices and %lu indices.\n",
               vertices_.size(), indices_.size());

    synch_buffers();

    poll_gl_errors();
}

void Mesh::append(const std::vector<Vertex>& vertices,
                  const std::vector<unsigned>& indices,
                  const glm::mat4 matrix) {
    poll_gl_errors();

    unsigned index_shift = (unsigned)vertices_.size();
    for (size_t vertex_id = 0; vertex_id < vertices.size(); ++vertex_id) {
        vertices_.push_back(vertices[vertex_id].apply(matrix));
    }
    for (size_t index_id = 0; index_id < indices.size(); ++index_id) {
        indices_.push_back(indices[index_id] + index_shift);
    }

    poll_gl_errors();
}

void Mesh::synch_buffers() {
    poll_gl_errors();

    vao_.bind();

    poll_gl_errors();

    vbo_.bind();
    vbo_.fill(&vertices_[0], vertices_.size());
    // vbo_.unbind();

    poll_gl_errors();

    ebo_.bind();
    ebo_.fill(&indices_[0], indices_.size());
    // ebo_.unbind();

    poll_gl_errors();

    Vertex::configure();

    vao_.unbind();

    poll_gl_errors();
}

void Mesh::render(const glm::mat4& proj_matrix, const glm::mat4& obj_matrix,
                  const Shader& shader) const {
    vao_.bind();
    vbo_.bind();
    ebo_.bind();
    shader.set_uniform_mat4("projection", proj_matrix);
    shader.set_uniform_mat4("obj_tform", obj_matrix);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, 0);
    vao_.unbind();
    vbo_.unbind();
}
