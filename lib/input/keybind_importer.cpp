#include <tinyxml2.h>

#include "binary_input.h"
#include "input_controller.h"
#include "logger/logger.h"
#include "managers/asset_manager.h"

IMPORTER(BinaryInput, "keybind") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("keybind");

    if (element == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Invalid keybind descriptor header\n");
        return nullptr;
    }

    Asset<BinaryInput>* asset = new Asset<BinaryInput>();

    for (const tinyxml2::XMLElement* child =
             element->FirstChildElement("input");
         child != nullptr; child = child->NextSiblingElement("input")) {
        const char* code = child->Attribute("code");

        if (code == nullptr) {
            log_printf(WARNINGS, "warning",
                       "Input action code was not specified\n");
        }

        const InputAction* action = InputController::get_action(code);

        if (action == nullptr) {
            log_printf(WARNINGS, "warning",
                       "Input action \"%s\" does not exist\n", code);
        }

        asset->content.add_activator(*action);
    }

    return asset;
}
