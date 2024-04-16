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
#include "managers/asset_manager.h"
#include "static_mesh.h"
#include "xml/data_extractors.h"

XML_BASED_IMPORTER(Producer, "static_mesh") {
    glm::mat4 transform = demand<glm::mat4>(data, "transform", glm::mat4(1.0));

    std::string path = demand<std::string>(data, "source", "NOT_MENTIONED");

    const ComplexModel* model = AssetManager::request<ComplexModel>(path);

    if (model == nullptr) {
        ERROR("Failed to load underlying complex model \"%s\"\n", path.c_str());
    }

    return new Asset<Producer>([=](const glm::mat4& parent_tform) {
        Subcomponent<StaticMesh> component(*model);
        component->set_transform(parent_tform * transform);

        return component;
    });
}
