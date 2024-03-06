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

template <class Vec>
static Vec parse(const tinyxml2::XMLElement* element);

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

    const char* shader_path = nullptr;
    shader_xml->QueryStringAttribute("path", &shader_path);

    if (shader_path == nullptr) {
        log_printf(ERROR_REPORTS, "error", "Shader path not specified\n");
        return nullptr;
    }

    const Shader* shader = AssetManager::request<Shader>(shader_path);

    if (shader == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load the material shader\n");
        return nullptr;
    }

    Asset<Material>* material = new Asset<Material>(*shader);

    if (material == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to construct the material\n");
    }

    for (const tinyxml2::XMLElement* child = head->FirstChildElement();
         child != nullptr; child = child->NextSiblingElement()) {
        parse_uniform(material->content, child);
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
