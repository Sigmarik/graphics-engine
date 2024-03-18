#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdio.h>
#include <tinyxml2.h>

#include <assimp/Importer.hpp>

#include "graphics/objects/complex_model.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"

static Asset<ComplexModel>* load_complex(const char* path) {
    ComplexModel model;

    static Assimp::Importer import;

    const aiScene* scene = import.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs |
                  aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        log_printf(ERROR_REPORTS, "error", "Failed to load complex model %s\n",
                   path);
        return nullptr;
    }

    aiMesh** meshes = scene->mMeshes;

    for (size_t mesh_id = 0; mesh_id < scene->mNumMeshes; ++mesh_id) {
        aiMesh* mesh = meshes[mesh_id];

        const char* mesh_name = mesh->mName.C_Str();

        //* `_box_` objects define collision boxes and should not be imported
        if (strncmp(mesh_name, "_box_", 5) == 0) continue;

        const char* material_name =
            scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str();

        const Material* material =
            AssetManager::request<Material>(material_name);

        if (material == nullptr) {
            log_printf(WARNINGS, "warning",
                       "Failed to load material \"%s\" used by part \"%s\" of "
                       "the complex mesh \"%s\"\n",
                       material_name, mesh_name, path);
        }

        Asset<Mesh>* part_mesh = new Asset<Mesh>(*mesh);

        AssetManager::register_rogue(part_mesh);

        model.add_part(Model(part_mesh->content, *material), mesh_name);
    }

    return new Asset<ComplexModel>(model);
}

IMPORTER(ComplexModel, "obj") { return load_complex(path); }
IMPORTER(ComplexModel, "fbx") { return load_complex(path); }
IMPORTER(ComplexModel, "glb") { return load_complex(path); }
IMPORTER(ComplexModel, "gltf") { return load_complex(path); }

IMPORTER(Mesh, "obj") { return new Asset<Mesh>(path); }
IMPORTER(Mesh, "fbx") { return new Asset<Mesh>(path); }
IMPORTER(Mesh, "glb") { return new Asset<Mesh>(path); }
IMPORTER(Mesh, "gltf") { return new Asset<Mesh>(path); }

IMPORTER(Model, "model") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* head = doc.FirstChildElement("model");

    if (head == nullptr) {
        log_printf(ERROR_REPORTS, "error", "Invalid model descriptor header\n");
        return nullptr;
    }

    const tinyxml2::XMLElement* mesh_xml = head->FirstChildElement("mesh");
    const tinyxml2::XMLElement* material_xml =
        head->FirstChildElement("material");

    if (mesh_xml == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified model mesh path (missing `mesh` tag)\n");
        return nullptr;
    }
    if (material_xml == nullptr) {
        log_printf(
            ERROR_REPORTS, "error",
            "Unspecified model material path (missing `material` tag)\n");
        return nullptr;
    }

    const char* mesh_path = nullptr;
    const char* material_path = nullptr;

    mesh_xml->QueryStringAttribute("path", &mesh_path);
    material_xml->QueryStringAttribute("path", &material_path);

    if (mesh_path == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified model mesh path (missing `path` attribute)\n");
        return nullptr;
    }
    if (material_path == nullptr) {
        log_printf(
            ERROR_REPORTS, "error",
            "Unspecified model material path (missing `path` attribute)\n");
        return nullptr;
    }

    const Mesh* mesh = AssetManager::request<Mesh>(mesh_path);
    const Material* material = AssetManager::request<Material>(material_path);

    if (mesh == nullptr || material == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load model dependencies (mesh.has_value() = %d, "
                   "material.has_value() = %d)\n",
                   mesh != nullptr, material != nullptr);
        return nullptr;
    }

    return new Asset<Model>(*mesh, *material);
}
