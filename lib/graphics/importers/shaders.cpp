#include <tinyxml2.h>

#include "graphics/primitives/shader.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"

IMPORTER(Shader, "shader") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("shader");

    if (element == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Invalid shader descriptor header\n");
        return nullptr;
    }

    const tinyxml2::XMLElement* vsh_element = element->FirstChildElement("vsh");
    const tinyxml2::XMLElement* fsh_element = element->FirstChildElement("fsh");

    if (vsh_element == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified vertex shader path (missing `vsh` tag)\n");
        return nullptr;
    }
    if (fsh_element == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Unspecified fragment shader path (missing `fsh` tag)\n");
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
