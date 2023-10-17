#include "importers.h"

#include <tinyxml2.h>

REGISTER(TextureImporter)
REGISTER(ShaderImporter)

AbstractAsset* TextureImporter::import(const char* path) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("texture");

    if (element == nullptr) return nullptr;

    if (element->FirstChildElement("path") == nullptr) return nullptr;
    if (element->FirstChildElement("slot") == nullptr) return nullptr;

    unsigned slot = 0;

    const char* content_path = element->FirstChildElement("path")->GetText();
    element->FirstChildElement("slot")->QueryUnsignedText(&slot);

    return new TextureAsset(content_path, slot);
}

AbstractAsset* ShaderImporter::import(const char* path) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    const tinyxml2::XMLElement* element = doc.FirstChildElement("shader");

    if (element == nullptr) return nullptr;

    if (element->FirstChildElement("vsh") == nullptr) return nullptr;
    if (element->FirstChildElement("fsh") == nullptr) return nullptr;

    const char* vsh_name = element->FirstChildElement("vsh")->GetText();
    const char* fsh_name = element->FirstChildElement("fsh")->GetText();

    return new ShaderAsset(vsh_name, fsh_name);
}
