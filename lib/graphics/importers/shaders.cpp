#include <tinyxml2.h>

#include "graphics/primitives/shader.h"
#include "logger/logger.h"
#include "managers/importer.h"

XML_IMPORTER(Shader, "shader") {
    const tinyxml2::XMLElement* vsh_element = data.FirstChildElement("vsh");
    const tinyxml2::XMLElement* fsh_element = data.FirstChildElement("fsh");

    if (vsh_element == nullptr) {
        ERROR("Unspecified vertex shader path (missing `vsh` tag)\n");
        return nullptr;
    }
    if (fsh_element == nullptr) {
        ERROR("Unspecified fragment shader path (missing `fsh` tag)\n");
        return nullptr;
    }

    const char* vsh_name = nullptr;
    const char* fsh_name = nullptr;

    vsh_element->QueryStringAttribute("path", &vsh_name);
    fsh_element->QueryStringAttribute("path", &fsh_name);

    if (vsh_name == nullptr) {
        log_printf(
            ERROR_REPORTS, "error",
            "Unspecified vertex shader path (missing `path` attribute)\n");
        return nullptr;
    }
    if (fsh_name == nullptr) {
        log_printf(
            ERROR_REPORTS, "error",
            "Unspecified fragment shader path (missing `path` attribute)\n");
        return nullptr;
    }

    return new Asset<Shader>(vsh_name, fsh_name);
}

IMPORTER(Shader, "shader") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    const tinyxml2::XMLElement* data = doc.FirstChildElement("shader");

    if (data == nullptr) {
        ERROR("Could not find the \"shader\" tag in \"%s\"\n", path.c_str());
        return nullptr;
    }

    return XMLAssetImporter<Shader, "shader">::import(*data, flags);
}
