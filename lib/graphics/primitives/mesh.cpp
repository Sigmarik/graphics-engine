#include "mesh.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdio.h>

#include <assimp/Importer.hpp>

#include "logger/logger.h"

Mesh::Mesh(const char* path) { load(path); }

void Mesh::load(const char* path) {
    Assimp::Importer import;
    const aiScene* scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        log_printf(ERROR_REPORTS, "error", "Failed to load model %s", path);
        return;
    }

    aiMesh** meshes = scene->mMeshes;

    size_t index_shift = 0;

    for (size_t mesh_id = 0; mesh_id < scene->mNumMeshes; ++mesh_id) {
        aiMesh* mesh = meshes[mesh_id];
        size_t vertex_count = mesh->mNumVertices;

        for (size_t vrt_id = 0; vrt_id < mesh->mNumVertices; ++vrt_id) {
            aiVector3D pos = mesh->mVertices[vrt_id];
            aiVector3D normal = mesh->mNormals[vrt_id];
            aiVector3D uv = mesh->mTextureCoords[vrt_id][0];
            vertices_.push_back(
                (Vertex){.position = glm::vec3(pos.x, pos.y, pos.z),
                         .normal = glm::vec3(normal.x, normal.y, normal.z),
                         .uv = glm::vec2(uv.x, uv.y)});
        }

        for (size_t face_id = 0; face_id < mesh->mNumFaces; ++face_id) {
            aiFace face = mesh->mFaces[face_id];
            for (unsigned id = 0; id < 3; ++id) {
                indices_.push_back(face.mIndices[id] + (unsigned)index_shift);
            }
        }

        index_shift = vertices_.size();
    }
}

void Mesh::append(const std::vector<Vertex>& vertices,
                  const std::vector<unsigned>& indices,
                  const glm::mat4 matrix) {
    unsigned index_shift = (unsigned)vertices_.size();
    for (size_t vertex_id = 0; vertex_id < vertices.size(); ++vertex_id) {
        vertices_.push_back(vertices[vertex_id].apply(matrix));
    }
    for (size_t index_id = 0; index_id < indices.size(); ++index_id) {
        indices_.push_back(indices[index_id] + index_shift);
    }
}

void Mesh::synch_buffers() {
    vao_.bind();

    vbo_.bind();
    vbo_.fill(&vertices_.front(), vertices_.size());
    vbo_.unbind();

    ebo_.bind();
    ebo_.fill(&indices_.front(), indices_.size());
    ebo_.unbind();

    Vertex::configure();

    vao_.unbind();
}

void Mesh::render(const glm::mat4& matrix) const {
    vao_.bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices_.size(), GL_UNSIGNED_INT, 0);
    vao_.unbind();
}
