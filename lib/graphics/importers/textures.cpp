#include <tinyxml2.h>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "graphics/primitives/texture.h"
#include "logger/logger.h"
#include "managers/importer.h"

IMPORTER(Texture, "png") { return new Asset<Texture>(path.c_str()); }
IMPORTER(Texture, "jpg") { return new Asset<Texture>(path.c_str()); }
IMPORTER(Texture, "bmp") { return new Asset<Texture>(path.c_str()); }

void read_wrap(const tinyxml2::XMLElement* element, TextureSettings& settings) {
    if (element == nullptr) return;

    const char* name = nullptr;

    element->QueryStringAttribute("mode", &name);

    if (name == nullptr) {
        log_printf(WARNINGS, "warning",
                   "Wrap mode not specified (missing `mode` attribute)\n");
        return;
    }

    if (strcmp(name, "repeat") == 0) {
        settings.wrap = GL_REPEAT;
    } else if (strcmp(name, "mirrored_repeat") == 0) {
        settings.wrap = GL_MIRRORED_REPEAT;
    } else if (strcmp(name, "clamp_to_edge") == 0) {
        settings.wrap = GL_CLAMP_TO_EDGE;
    } else if (strcmp(name, "clamp_to_border") == 0) {
        settings.wrap = GL_CLAMP_TO_BORDER;
    } else {
        log_printf(WARNINGS, "warning", "Invalid wrap mode \"%s\"\n", name);
    }

    // CLAMP_TO_BORDER fills unmapped pixels of the image with BORDER_COLOR
    // This is where the user can define the color they want to use

    // <wrap mode="clamp_to_border" r="1.0" g="0.0" b="1.0"></wrap>
    //                            ^^^^^^^^^^^^^^^^^^^^^^^

    element->QueryFloatAttribute("r", &settings.color.r);
    element->QueryFloatAttribute("g", &settings.color.g);
    element->QueryFloatAttribute("b", &settings.color.b);

    element->QueryFloatAttribute("red", &settings.color.r);
    element->QueryFloatAttribute("green", &settings.color.g);
    element->QueryFloatAttribute("blue", &settings.color.b);

    float red = settings.color.r;
    float green = settings.color.g;
    float blue = settings.color.b;

    if (red < 0.0 || red > 1.0 || green < 0.0 || green > 1.0 || blue < 0.0 ||
        blue > 1.0) {
        settings.color.r = glm::clamp(settings.color.r, 0.0f, 1.0f);
        settings.color.g = glm::clamp(settings.color.g, 0.0f, 1.0f);
        settings.color.b = glm::clamp(settings.color.b, 0.0f, 1.0f);

        log_printf(WARNINGS, "warning",
                   "Clamping invalid border color RGB(%g, %g, %g) to "
                   "RGB(%g, %g, %g)\n",
                   red, green, blue, settings.color.r, settings.color.g,
                   settings.color.b);
    }
}

void read_interp(const tinyxml2::XMLElement* element,
                 TextureSettings& settings) {
    if (element == nullptr) return;

    const char* name = nullptr;

    element->QueryStringAttribute("mode", &name);

    if (name == nullptr) {
        log_printf(
            WARNINGS, "warning",
            "Interpolation mode not specified (missing `mode` attribute)\n");
        return;
    }

    if (strcmp(name, "linear") == 0) {
        settings.interp = GL_LINEAR;
    } else if (strcmp(name, "nearest") == 0) {
        settings.interp = GL_NEAREST;
    } else {
        log_printf(WARNINGS, "warning", "Invalid interpolation mode \"%s\"\n",
                   name);
    }
}

XML_IMPORTER(Texture, "texture") {
    const tinyxml2::XMLElement* path_element = data.FirstChildElement("file");

    if (path_element == nullptr) {
        ERROR("Unspecified texture file (missing `file` tag)\n");
        return nullptr;
    }

    const char* content_path = nullptr;
    path_element->QueryStringAttribute("path", &content_path);

    if (content_path == nullptr) {
        ERROR("Unspecified texture file path (missing `path` attribute)\n");
        return nullptr;
    }

    Asset<Texture>* asset = new Asset<Texture>(content_path);

    TextureSettings settings = {.wrap = GL_REPEAT, .interp = GL_NEAREST};

    read_wrap(data.FirstChildElement("wrap"), settings);
    read_interp(data.FirstChildElement("interp"), settings);

    asset->content.use_settings(settings);

    return asset;
}

IMPORTER(Texture, "texture") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    const tinyxml2::XMLElement* data = doc.FirstChildElement("texture");

    if (data == nullptr) {
        ERROR("Could not find the \"texture\" tag in \"%s\"\n", path.c_str());
        return nullptr;
    }

    return XMLAssetImporter<Texture, "texture">::import(*data, flags);
}
