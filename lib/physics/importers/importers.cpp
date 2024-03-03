#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <tinyxml2.h>

#include <assimp/Importer.hpp>
#include <optional>
#include <utility>

#include "logger/logger.h"
#include "managers/asset_manager.h"
#include "physics/level_geometry.h"

static const double MERGE_DISTANCE = 1e-4;

static void remove_identical(std::vector<glm::vec3>& vertices) {
    size_t unique_count = 0;

    for (size_t id = 0; id < vertices.size(); ++id) {
        glm::vec3 current = vertices[id];

        bool unique = true;

        for (size_t pattern = 0; pattern < unique_count; ++pattern) {
            unique &=
                glm::distance(vertices[pattern], current) > MERGE_DISTANCE;
        }

        if (unique) {
            std::swap(vertices[id], vertices[unique_count]);
            ++unique_count;
        }
    }

    vertices.resize(unique_count);
}

static std::optional<glm::mat3> compute_bounds(
    const std::vector<glm::vec3>& vertices) {
    glm::vec3 origin = vertices[0];

    glm::vec3 arm_x = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 arm_y = glm::vec3(0.0, 0.0, 0.0);

    for (glm::vec3 vertex : vertices) {
        glm::vec3 arm = vertex - origin;

        if (glm::length(arm) < glm::length(arm_x) ||
            glm::length(arm_x) < MERGE_DISTANCE) {
            arm_y = arm_x;
            arm_x = arm;
        } else if (glm::length(arm) < glm::length(arm_y) ||
                   glm::length(arm_y) < MERGE_DISTANCE) {
            arm_y = arm;
        }
    }

    glm::vec3 arm_z = glm::vec3(0.0, 0.0, 0.0);

    glm::vec3 normal = glm::cross(arm_x, arm_y);
    if (glm::length(normal) < MERGE_DISTANCE) {
        return {};
    }

    normal = glm::normalize(normal);

    for (glm::vec3 vertex : vertices) {
        glm::vec3 arm = normal * glm::dot(normal, vertex - origin);

        if (glm::length(arm) > glm::length(arm_z)) {
            arm_z = arm;
        }
    }

    if (glm::dot(glm::cross(arm_x, arm_y), arm_z) < 0.0) {
        std::swap(arm_x, arm_y);
    }

    return glm::mat3(arm_x, arm_y, arm_z);
}

static std::optional<BoxCollider> parse_box(
    const std::vector<glm::vec3>& vertices) {
    if (vertices.size() != 8) {
        log_printf(WARNINGS, "warning",
                   "Incorrect number of unique vertices (%lu instead of 8)\n",
                   vertices.size());
        return {};
    }

    glm::vec3 origin = vertices[0];

    std::optional<glm::mat3> boundary = compute_bounds(vertices);

    if (!boundary) {
        log_printf(WARNINGS, "warning", "Failed to determine box axises\n");
        return {};
    }

    glm::vec3 arm_x = boundary->operator[](0);
    glm::vec3 arm_y = boundary->operator[](1);
    glm::vec3 arm_z = boundary->operator[](2);

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

            vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
        }

        remove_identical(vertices);

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
