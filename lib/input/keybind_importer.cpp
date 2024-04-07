#include <tinyxml2.h>

#include "binary_input.h"
#include "input_controller.h"
#include "logger/logger.h"
#include "managers/importer.h"

XML_BASED_IMPORTER(BinaryInput, "keybind") {
    Asset<BinaryInput>* asset = new Asset<BinaryInput>();

    for (const tinyxml2::XMLElement* child = data.FirstChildElement("input");
         child != nullptr; child = child->NextSiblingElement("input")) {
        const char* code = child->Attribute("code");

        if (code == nullptr) {
            WARNING("Input action code was not specified\n");
        }

        const InputAction* action = InputController::get_action(code);

        if (action == nullptr) {
            WARNING("Input action \"%s\" does not exist\n", code);
        }

        asset->content.add_activator(*action);
    }

    return asset;
}
