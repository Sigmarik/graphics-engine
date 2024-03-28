#include <tinyxml2.h>

#include "graphics/objects/material.h"
#include "logger/logger.h"
#include "managers/importer.h"

template <class T>
static T parse(const tinyxml2::XMLElement* element);

static void parse_uniform(Material& material,
                          const tinyxml2::XMLElement* element);

XML_IMPORTER(Material, "material") {
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

    const Shader* shader = AssetManager::request<Shader>(shader_path);

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

IMPORTER(Material, "material") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    const tinyxml2::XMLElement* data = doc.FirstChildElement("material");
    if (data == nullptr) {
        ERROR("Could not find the \"material\" tag in \"%s\"\n", path.c_str());
        return nullptr;
    }

    return XMLAssetImporter<Material, "material">::import(*data, flags);
}

template <>
float parse<float>(const tinyxml2::XMLElement* element) {
    assert(element);

    float value = 0.0f;

    element->QueryFloatAttribute("r", &value);
    element->QueryFloatAttribute("x", &value);
    element->QueryFloatAttribute("v", &value);
    element->QueryFloatAttribute("val", &value);
    element->QueryFloatAttribute("float", &value);
    element->QueryFloatAttribute("value", &value);

    return value;
}

template <>
int parse<int>(const tinyxml2::XMLElement* element) {
    assert(element);

    int value = 0;

    element->QueryIntAttribute("v", &value);
    element->QueryIntAttribute("val", &value);
    element->QueryIntAttribute("int", &value);
    element->QueryIntAttribute("value", &value);

    return value;
}

template <>
glm::vec2 parse<glm::vec2>(const tinyxml2::XMLElement* element) {
    assert(element);

    glm::vec2 value = glm::vec2(0.0);

    element->QueryFloatAttribute("r", &value.x);
    element->QueryFloatAttribute("g", &value.y);

    element->QueryFloatAttribute("x", &value.x);
    element->QueryFloatAttribute("y", &value.y);

    return value;
}

template <>
glm::vec3 parse<glm::vec3>(const tinyxml2::XMLElement* element) {
    assert(element);

    glm::vec3 value = glm::vec3(0.0);

    element->QueryFloatAttribute("r", &value.x);
    element->QueryFloatAttribute("g", &value.y);
    element->QueryFloatAttribute("b", &value.z);

    element->QueryFloatAttribute("x", &value.x);
    element->QueryFloatAttribute("y", &value.y);
    element->QueryFloatAttribute("z", &value.z);

    return value;
}

template <>
glm::vec4 parse<glm::vec4>(const tinyxml2::XMLElement* element) {
    assert(element);

    glm::vec4 value = glm::vec4(0.0);

    element->QueryFloatAttribute("r", &value.x);
    element->QueryFloatAttribute("g", &value.y);
    element->QueryFloatAttribute("b", &value.z);
    element->QueryFloatAttribute("a", &value.w);

    element->QueryFloatAttribute("x", &value.x);
    element->QueryFloatAttribute("y", &value.y);
    element->QueryFloatAttribute("z", &value.z);
    element->QueryFloatAttribute("w", &value.w);

    return value;
}

template <>
glm::mat2 parse<glm::mat2>(const tinyxml2::XMLElement* element) {
    const tinyxml2::XMLElement* x_descr = nullptr;
    const tinyxml2::XMLElement* y_descr = nullptr;

    x_descr = element->FirstChildElement("dx");
    y_descr = element->FirstChildElement("dy");

    if (x_descr == nullptr || y_descr == nullptr) {
        log_printf(WARNINGS, "warning", "Missing matrix column\n");
        return glm::mat2(1.0);
    }

    return glm::mat2(parse<glm::vec2>(x_descr), parse<glm::vec2>(y_descr));
}

template <>
glm::mat3 parse<glm::mat3>(const tinyxml2::XMLElement* element) {
    const tinyxml2::XMLElement* x_descr = nullptr;
    const tinyxml2::XMLElement* y_descr = nullptr;
    const tinyxml2::XMLElement* z_descr = nullptr;

    x_descr = element->FirstChildElement("dx");
    y_descr = element->FirstChildElement("dy");
    z_descr = element->FirstChildElement("dz");

    if (x_descr == nullptr || y_descr == nullptr || z_descr == nullptr) {
        log_printf(WARNINGS, "warning", "Missing matrix column\n");
        return glm::mat3(1.0);
    }

    return glm::mat3(parse<glm::vec3>(x_descr), parse<glm::vec3>(y_descr),
                     parse<glm::vec3>(z_descr));
}

template <>
glm::mat4 parse<glm::mat4>(const tinyxml2::XMLElement* element) {
    const tinyxml2::XMLElement* x_descr = nullptr;
    const tinyxml2::XMLElement* y_descr = nullptr;
    const tinyxml2::XMLElement* z_descr = nullptr;
    const tinyxml2::XMLElement* w_descr = nullptr;

    x_descr = element->FirstChildElement("dx");
    y_descr = element->FirstChildElement("dy");
    z_descr = element->FirstChildElement("dz");
    w_descr = element->FirstChildElement("dw");

    if (x_descr == nullptr || y_descr == nullptr || z_descr == nullptr ||
        w_descr == nullptr) {
        log_printf(WARNINGS, "warning", "Missing matrix column\n");
        return glm::mat4(1.0);
    }

    return glm::mat4(parse<glm::vec4>(x_descr), parse<glm::vec4>(y_descr),
                     parse<glm::vec4>(z_descr), parse<glm::vec4>(w_descr));
}

template <>
const Texture* parse<const Texture*>(const tinyxml2::XMLElement* element) {
    const char* path = nullptr;

    element->QueryStringAttribute("path", &path);

    if (path == nullptr) {
        log_printf(WARNINGS, "warning", "Texture path not specified\n");
        return nullptr;
    }

    return AssetManager::request<Texture>(path);
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
        material.set_uniform(uniform_name, parse<int>(element));
    } else if (strcmp(element->Name(), "float") == 0) {
        material.set_uniform(uniform_name, parse<float>(element));
    } else if (strcmp(element->Name(), "vec2") == 0) {
        material.set_uniform(uniform_name, parse<glm::vec2>(element));
    } else if (strcmp(element->Name(), "vec3") == 0) {
        material.set_uniform(uniform_name, parse<glm::vec3>(element));
    } else if (strcmp(element->Name(), "vec4") == 0) {
        material.set_uniform(uniform_name, parse<glm::vec4>(element));
    } else if (strcmp(element->Name(), "mat2") == 0) {
        material.set_uniform(uniform_name, parse<glm::mat2>(element));
    } else if (strcmp(element->Name(), "mat3") == 0) {
        material.set_uniform(uniform_name, parse<glm::mat3>(element));
    } else if (strcmp(element->Name(), "mat4") == 0) {
        material.set_uniform(uniform_name, parse<glm::mat4>(element));
    } else if (strcmp(element->Name(), "texture") == 0) {
        const Texture* texture = parse<const Texture*>(element);

        if (texture == nullptr) {
            log_printf(WARNINGS, "warning", "Failed to load uniform texture\n");
        } else {
            material.set_uniform(uniform_name, texture);
        }
    }
}
