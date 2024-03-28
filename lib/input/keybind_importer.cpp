#include <tinyxml2.h>

#include "binary_input.h"
#include "input_controller.h"
#include "logger/logger.h"
#include "managers/importer.h"

XML_IMPORTER(BinaryInput, "keybind") {
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

IMPORTER(BinaryInput, "keybind") {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());

    const tinyxml2::XMLElement* data = doc.FirstChildElement("keybind");

    if (data == nullptr) {
        ERROR("Could not find the \"keybind\" tag in \"%s\"\n", path.c_str());
        return nullptr;
    }

    return XMLAssetImporter<BinaryInput, "keybind">::import(*data, flags);
}
