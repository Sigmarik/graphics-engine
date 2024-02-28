#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <tinyxml2.h>

#include <assimp/Importer.hpp>
#include <optional>
#include <utility>

#include "logger/logger.h"
#include "managers/asset_manager.h"
#include "physics/level_geometry.h"

static std::optional<BoxCollider> parse_box(
    const std::vector<glm::vec3>& vertices) {
    if (vertices.size() < 8) {
        log_printf(WARNINGS, "warning", "Not enough vertices\n");
        return {};
    }

    glm::vec3 origin = vertices[0];

    glm::vec3 arm_x = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 arm_y = glm::vec3(0.0, 0.0, 0.0);

    for (glm::vec3 vertex : vertices) {
        glm::vec3 arm = vertex - origin;

        if (glm::length(arm) < 1e-4) continue;

        if (glm::length(arm) < glm::length(arm_x) ||
            glm::length(arm_x) < 1e-4) {
            arm_y = arm_x;
            arm_x = arm;
        } else if (glm::length(arm) < glm::length(arm_y) ||
                   glm::length(arm_y) < 1e-4) {
            arm_y = arm;
        }
    }

    glm::vec3 arm_z = glm::vec3(0.0, 0.0, 0.0);

    glm::vec3 normal = glm::cross(arm_x, arm_y);
    if (glm::length(normal) < 1e-4) {
        log_printf(WARNINGS, "warning",
                   "Base vectors produce zero determinant\n");
        return {};
    }

    normal = glm::normalize(normal);

    for (glm::vec3 vertex : vertices) {
        glm::vec3 arm = normal * glm::dot(normal, vertex - origin);

        if (glm::length(arm) > glm::length(arm_z)) {
            arm_z = arm;
        }
    }

    if (glm::dot(glm::cross(arm_x, arm_y), arm_z) > 0.0) {
        std::swap(arm_x, arm_y);
    }

    glm::vec3 size(glm::length(arm_x), glm::length(arm_y), glm::length(arm_z));

    Box box(size / 2.0f, size);

    glm::vec4 x_axis(glm::normalize(arm_x), 0.0f);
    glm::vec4 y_axis(glm::normalize(arm_y), 0.0f);
    glm::vec4 z_axis(glm::normalize(arm_z), 0.0f);
    glm::vec4 w_axis(origin, 1.0f);

    glm::mat4 transform = glm::mat4(x_axis, y_axis, z_axis, w_axis);

    return BoxCollider(box, transform);
}

IMPORTER(CollisionGroup, "obj") {
    static Assimp::Importer import;

    CollisionGroup group = {};

    const aiScene* scene =
        import.ReadFile(path, aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        log_printf(ERROR_REPORTS, "error", "Failed to load collision for %s\n",
                   path);
        return nullptr;
    }

    aiMesh** meshes = scene->mMeshes;

    for (size_t mesh_id = 0; mesh_id < scene->mNumMeshes; ++mesh_id) {
        aiMesh* mesh = meshes[mesh_id];

        if (strncmp(mesh->mName.C_Str(), "_box_", 5) != 0) continue;

        size_t vertex_count = mesh->mNumVertices;

        std::vector<glm::vec3> vertices;

        for (size_t vrt_id = 0; vrt_id < vertex_count; ++vrt_id) {
            aiVector3D pos = mesh->mVertices[vrt_id];

            glm::vec3 new_vertex = glm::vec3(pos.x, pos.y, pos.z);

            bool duplicate = false;
            for (const glm::vec3& vertex : vertices) {
                if (glm::distance(vertex, new_vertex) < 1e-4) duplicate = true;
            }

            if (duplicate) continue;

            vertices.push_back(new_vertex);
        }

        std::optional<BoxCollider> collider = parse_box(vertices);

        if (collider.has_value()) {
            group.push_back(collider.value());
        } else {
            log_printf(WARNINGS, "warning",
                       "Collision box \"%s\" (file %s) is invalid\n",
                       mesh->mName.C_Str(), path);
        }
    }

    return new Asset<CollisionGroup>(group);
}
