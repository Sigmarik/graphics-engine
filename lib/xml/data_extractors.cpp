#include "data_extractors.h"

#include <assert.h>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "graphics/primitives/texture.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"

template <>
float extract<float>(const tinyxml2::XMLElement* element) {
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
int extract<int>(const tinyxml2::XMLElement* element) {
    assert(element);

    int value = 0;

    element->QueryIntAttribute("v", &value);
    element->QueryIntAttribute("val", &value);
    element->QueryIntAttribute("int", &value);
    element->QueryIntAttribute("value", &value);

    return value;
}

template <>
bool extract<bool>(const tinyxml2::XMLElement* element) {
    assert(element);

    bool value = 0;

    element->QueryBoolAttribute("v", &value);
    element->QueryBoolAttribute("val", &value);
    element->QueryBoolAttribute("bool", &value);
    element->QueryBoolAttribute("value", &value);
    element->QueryBoolAttribute("active", &value);
    element->QueryBoolAttribute("checked", &value);

    return value;
}

template <>
glm::vec2 extract<glm::vec2>(const tinyxml2::XMLElement* element) {
    assert(element);

    glm::vec2 value = glm::vec2(0.0);

    element->QueryFloatAttribute("r", &value.x);
    element->QueryFloatAttribute("g", &value.y);

    element->QueryFloatAttribute("x", &value.x);
    element->QueryFloatAttribute("y", &value.y);

    return value;
}

template <>
glm::vec3 extract<glm::vec3>(const tinyxml2::XMLElement* element) {
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
glm::vec4 extract<glm::vec4>(const tinyxml2::XMLElement* element) {
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
glm::mat2 extract<glm::mat2>(const tinyxml2::XMLElement* element) {
    const tinyxml2::XMLElement* x_descr = nullptr;
    const tinyxml2::XMLElement* y_descr = nullptr;

    x_descr = element->FirstChildElement("dx");
    y_descr = element->FirstChildElement("dy");

    if (x_descr == nullptr || y_descr == nullptr) {
        log_printf(WARNINGS, "warning", "Missing matrix column\n");
        return glm::mat2(1.0);
    }

    return glm::mat2(extract<glm::vec2>(x_descr), extract<glm::vec2>(y_descr));
}

template <>
glm::mat3 extract<glm::mat3>(const tinyxml2::XMLElement* element) {
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

    return glm::mat3(extract<glm::vec3>(x_descr), extract<glm::vec3>(y_descr),
                     extract<glm::vec3>(z_descr));
}

template <>
glm::mat4 extract<glm::mat4>(const tinyxml2::XMLElement* element) {
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

    return glm::mat4(extract<glm::vec4>(x_descr), extract<glm::vec4>(y_descr),
                     extract<glm::vec4>(z_descr), extract<glm::vec4>(w_descr));
}

template <>
const Texture* extract<const Texture*>(const tinyxml2::XMLElement* element) {
    const char* path = nullptr;

    element->QueryStringAttribute("path", &path);

    if (path == nullptr) {
        log_printf(WARNINGS, "warning", "Texture path not specified\n");
        return nullptr;
    }

    return AssetManager::request<Texture>(path, "texture");
}

template <>
std::string extract<std::string>(const tinyxml2::XMLElement* element) {
    const char* line = nullptr;

    element->QueryStringAttribute("name", &line);
    element->QueryStringAttribute("path", &line);
    element->QueryStringAttribute("string", &line);
    element->QueryStringAttribute("str", &line);
    element->QueryStringAttribute("id", &line);
    element->QueryStringAttribute("guid", &line);

    return std::string(line ? line : "");
}
