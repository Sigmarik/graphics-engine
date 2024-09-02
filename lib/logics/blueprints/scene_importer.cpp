/**
 * @file scene_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Scene importer
 * @version 0.1
 * @date 2024-04-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "external_level.h"
#include "managers/importer.h"

XML_BASED_IMPORTER(ExternalLevel, "level") {
    ExternalLevel::Factory factory;
    std::vector<Script> scripts;

    const ExternalLevel::Metadata* meta = nullptr;

    for (const tinyxml2::XMLElement* child = data.FirstChildElement();
         child != nullptr; child = child->NextSiblingElement()) {
        if (strcmp(child->Name(), "script") == 0) {
            const Script* script = AssetManager::request<Script>(*child);

            if (!script) {
                log_printf(ERROR_REPORTS, "error",
                           "Failed to load an underlying script.\n");
                continue;
            }

            scripts.push_back(*script);

            continue;
        }

        if (child->Attribute("type", "meta")) {
            meta = AssetManager::request<ExternalLevel::Metadata>(*child);
            continue;
        }

        const char* name = nullptr;
        child->QueryStringAttribute("name", &name);

        if (name == nullptr) {
            ERROR(
                "Unnamed scene component of type \"%s\", missing `name` "
                "attribute\n",
                child->Name());
            continue;
        }

        const ExternalLevel::Factory::Producer* producer = AssetManager::
            request<ExternalLevel::Factory::Producer>(*child);

        if (producer == nullptr) {
            ERROR("Failed to request a producer for an object type of \"%s\"\n",
                  child->Name());
            continue;
        }

        factory.register_producer(name, *producer);
    }

    return new Asset<ExternalLevel>(factory, scripts, meta);
}
