#include <tinyxml2.h>

#include "graphics/objects/material.h"
#include "graphics/objects/model.h"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"

IMPORTER(Texture, "texture") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("texture");

    if (element == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Invalid texture descriptor header\n");
        return nullptr;
    }

    if (element->FirstChildElement("path") == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified texture path (`path` tag)\n");
        return nullptr;
    }
    if (element->FirstChildElement("slot") == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified texture slot (`slot` tag)\n");
        return nullptr;
    }

    unsigned slot = 0;

    const char* content_path =
        trim_path(element->FirstChildElement("path")->GetText());
    element->FirstChildElement("slot")->QueryUnsignedText(&slot);

    return new Asset<Texture>(content_path, slot);
}

IMPORTER(Shader, "shader") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("shader");

    if (element == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Invalid shader descriptor header\n");
        return nullptr;
    }

    if (element->FirstChildElement("vsh") == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified vertex shader path (`vsh` tag)\n");
        return nullptr;
    }
    if (element->FirstChildElement("fsh") == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified fragment shader path (`fsh` tag)\n");
        return nullptr;
    }

    static char vsh_name[PATH_LENGTH] = "";
    static char fsh_name[PATH_LENGTH] = "";

    copy_trimmed(vsh_name, element->FirstChildElement("vsh")->GetText());
    copy_trimmed(fsh_name, element->FirstChildElement("fsh")->GetText());

    return new Asset<Shader>(vsh_name, fsh_name);
}

IMPORTER(Material, "material") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* head = doc.FirstChildElement("material");

    if (head == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Invalid material descriptor header\n");
        return nullptr;
    }

    const tinyxml2::XMLElement* shader_xml = head->FirstChildElement("shader");

    if (shader_xml == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified shader descriptor path (`shader` tag)\n");
        return nullptr;
    }

    const Shader* shader =
        AssetManager::request<Shader>(trim_path(shader_xml->GetText()));

    if (shader == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load the material shader\n");
        return nullptr;
    }

    Asset<Material>* material = new Asset<Material>(*shader);

    for (const tinyxml2::XMLElement* child = head->FirstChildElement();
         child != nullptr; child = child->NextSiblingElement()) {
        if (strcmp(child->Name(), "texture") == 0) {
            const Texture* texture =
                AssetManager::request<Texture>(trim_path(child->GetText()));

            if (texture == nullptr) {
                log_printf(WARNINGS, "warning",
                           "Failed to load texture file while setting up the "
                           "material\n");
                continue;
            }

            const char* uniform_name = child->Attribute("uniform");

            if (uniform_name == nullptr) {
                log_printf(WARNINGS, "warning",
                           "Unspecified texture uniform name (could not find a "
                           "`uniform` attribute)\n");
                continue;
            }

            material->content.set_uniform(uniform_name, *texture);
        }
    }

    return material;
}

IMPORTER(Mesh, "obj") { return new Asset<Mesh>(path); }

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
                   "Unspecified model mesh path (`mesh` tag)\n");
        return nullptr;
    }
    if (material_xml == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified model material path (`material` tag)\n");
        return nullptr;
    }

    const Mesh* mesh =
        AssetManager::request<Mesh>(trim_path(mesh_xml->GetText()));
    const Material* material =
        AssetManager::request<Material>(trim_path(material_xml->GetText()));

    if (mesh == nullptr || material == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load model dependencies (mesh.has_value() = %d, "
                   "material.has_value() = %d)\n",
                   mesh != nullptr, material != nullptr);
        return nullptr;
    }

    return new Asset<Model>(*mesh, *material);
}
