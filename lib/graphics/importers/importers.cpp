#include <tinyxml2.h>

#include "graphics/objects/material.h"
#include "graphics/objects/model.h"
#include "graphics/primitives/mesh.h"
#include "graphics/primitives/shader.h"
#include "graphics/primitives/texture.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"

IMPORTER(Texture, texture) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("texture");

    if (element == nullptr) return nullptr;

    if (element->FirstChildElement("path") == nullptr) return nullptr;
    if (element->FirstChildElement("slot") == nullptr) return nullptr;

    unsigned slot = 0;

    const char* content_path =
        trim_path(element->FirstChildElement("path")->GetText());
    element->FirstChildElement("slot")->QueryUnsignedText(&slot);

    return new Asset<Texture>(content_path, slot);
}

IMPORTER(Shader, shader) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("shader");

    if (element == nullptr) return nullptr;

    if (element->FirstChildElement("vsh") == nullptr) return nullptr;
    if (element->FirstChildElement("fsh") == nullptr) return nullptr;

    static char vsh_name[PATH_LENGTH] = "";
    static char fsh_name[PATH_LENGTH] = "";

    copy_trimmed(vsh_name, element->FirstChildElement("vsh")->GetText());
    copy_trimmed(fsh_name, element->FirstChildElement("fsh")->GetText());

    return new Asset<Shader>(vsh_name, fsh_name);
}

IMPORTER(Material, material) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* head = doc.FirstChildElement("material");

    if (head == nullptr) return nullptr;

    const tinyxml2::XMLElement* shader_xml = head->FirstChildElement("shader");

    if (shader_xml == nullptr) return nullptr;

    const Shader* shader =
        AssetManager::request<Shader>(trim_path(shader_xml->GetText()));

    if (shader == nullptr) return nullptr;

    Asset<Material>* material = new Asset<Material>(*shader);

    for (const tinyxml2::XMLElement* child = head->FirstChildElement();
         child != nullptr; child = child->NextSiblingElement()) {
        if (strcmp(child->Name(), "texture") == 0) {
            const Texture* texture =
                AssetManager::request<Texture>(trim_path(child->GetText()));

            if (texture == nullptr) return nullptr;

            const char* uniform_name = child->Attribute("uniform");

            if (uniform_name == nullptr) return nullptr;

            material->content.add_texture(uniform_name, texture);
        }
    }

    return material;
}

IMPORTER(Mesh, obj) { return new Asset<Mesh>(path); }

IMPORTER(Model, model) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* head = doc.FirstChildElement("model");

    if (head == nullptr) return nullptr;

    const tinyxml2::XMLElement* mesh_xml = head->FirstChildElement("mesh");
    const tinyxml2::XMLElement* material_xml =
        head->FirstChildElement("material");

    if (mesh_xml == nullptr) return nullptr;
    if (material_xml == nullptr) return nullptr;

    const Mesh* mesh =
        AssetManager::request<Mesh>(trim_path(mesh_xml->GetText()));
    const Material* material =
        AssetManager::request<Material>(trim_path(material_xml->GetText()));

    return new Asset<Model>(*mesh, *material);
}
