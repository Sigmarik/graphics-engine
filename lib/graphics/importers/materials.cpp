#include <tinyxml2.h>

#include "graphics/objects/material.h"
#include "logger/logger.h"
#include "managers/importer.h"
#include "xml/data_extractors.h"

static void parse_uniform(Material& material,
                          const tinyxml2::XMLElement* element);

XML_BASED_IMPORTER(Material, "material") {
    const tinyxml2::XMLElement* shader_xml = data.FirstChildElement("shader");

    if (shader_xml == nullptr) {
        log_printf(
            ERROR_REPORTS, "error",
            "Unspecified shader descriptor path (missing `shader` tag)\n");
        return nullptr;
    }

    const char* shader_path = nullptr;
    shader_xml->QueryStringAttribute("path", &shader_path);

    if (shader_path == nullptr) {
        ERROR("Shader path not specified (missing `path` attribute)\n");
        return nullptr;
    }

    const Shader* shader = AssetManager::request<Shader>(shader_path, "shader");

    if (shader == nullptr) {
        ERROR("Failed to load shader \"%s\"\n", shader_path);
        return nullptr;
    }

    Asset<Material>* material = new Asset<Material>(*shader);

    if (material == nullptr) {
        ERROR("Failed to construct the material from shader \"%s\"\n",
              shader_path);
    }

    for (const tinyxml2::XMLElement* child = data.FirstChildElement();
         child != nullptr; child = child->NextSiblingElement()) {
        parse_uniform(material->content, child);
    }

    return material;
}

static void parse_uniform(Material& material,
                          const tinyxml2::XMLElement* element) {
    assert(element);

    const char* uniform_name = nullptr;

    element->QueryStringAttribute("uni", &uniform_name);
    element->QueryStringAttribute("uform", &uniform_name);
    element->QueryStringAttribute("var", &uniform_name);
    element->QueryStringAttribute("variable", &uniform_name);
    element->QueryStringAttribute("name", &uniform_name);
    element->QueryStringAttribute("uniform", &uniform_name);

    if (uniform_name == nullptr) return;

    if (strcmp(element->Name(), "int") == 0) {
        material.set_uniform(uniform_name, extract<int>(element));
    } else if (strcmp(element->Name(), "float") == 0) {
        material.set_uniform(uniform_name, extract<float>(element));
    } else if (strcmp(element->Name(), "vec2") == 0) {
        material.set_uniform(uniform_name, extract<glm::vec2>(element));
    } else if (strcmp(element->Name(), "vec3") == 0) {
        material.set_uniform(uniform_name, extract<glm::vec3>(element));
    } else if (strcmp(element->Name(), "vec4") == 0) {
        material.set_uniform(uniform_name, extract<glm::vec4>(element));
    } else if (strcmp(element->Name(), "mat2") == 0) {
        material.set_uniform(uniform_name, extract<glm::mat2>(element));
    } else if (strcmp(element->Name(), "mat3") == 0) {
        material.set_uniform(uniform_name, extract<glm::mat3>(element));
    } else if (strcmp(element->Name(), "mat4") == 0) {
        material.set_uniform(uniform_name, extract<glm::mat4>(element));
    } else if (strcmp(element->Name(), "texture") == 0) {
        const Texture* texture = extract<const Texture*>(element);

        if (texture == nullptr) {
            log_printf(WARNINGS, "warning", "Failed to load uniform texture\n");
        } else {
            material.set_uniform(uniform_name, texture);
        }
    }
}
