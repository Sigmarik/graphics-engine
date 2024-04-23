/**
 * @file static_mesh_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Static mesh component importer
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "logics/blueprints/component_importer.h"
#include "logics/components/logical/component_pack.h"
#include "logics/components/physical/static_box_collider.h"
#include "managers/asset_manager.h"
#include "static_mesh.h"
#include "xml/data_extractors.h"

static BoxCollider transform_collider(const BoxCollider& collider,
                                      const glm::mat4& transform) {
    glm::mat4 required_transform = transform * collider.get_transform();
    Box box = collider.get_box();

    glm::vec4 size_proto = required_transform * glm::vec4(1.0, 1.0, 1.0, 0.0);

    glm::vec3 size(size_proto.x, size_proto.y, size_proto.z);

    // clang-format off
            glm::mat4 descale(1.0 / size.x, 0.0, 0.0, 0.0,
                              0.0, 1.0 / size.y, 0.0, 0.0,
                              0.0, 0.0, 1.0 / size.z, 0.0,
                              0.0, 0.0, 0.0, 1.0);
    // clang-format on

    return BoxCollider(Box(glm::vec3(0.0), box.get_size() * size),
                       required_transform * descale);
}

XML_BASED_IMPORTER(Producer, "static_mesh") {
    glm::mat4 transform = demand<glm::mat4>(data, "transform", glm::mat4(1.0));

    std::string path = demand<std::string>(data, "source", "NOT_MENTIONED");

    const ComplexModel* model = AssetManager::request<ComplexModel>(path);

    if (model == nullptr) {
        ERROR("Failed to load underlying complex model \"%s\"\n", path.c_str());
    }

    bool import_collision = request<bool>(data, "collision", false);

    return new Asset<Producer>(PRODUCER {
        Subcomponent<StaticMesh> mesh(*model);
        mesh->set_transform(parent_tform * transform);

        if (!import_collision) return mesh;

        Subcomponent<ComponentPack> pack;

        pack->add_component(mesh);

        const CollisionGroup* colliders =
            AssetManager::request<CollisionGroup>(path);

        for (BoxCollider collider : *colliders) {
            pack->add_component(Subcomponent<StaticBoxCollider>(
                transform_collider(collider, parent_tform * transform)));
        }

        return pack;
    });
}
