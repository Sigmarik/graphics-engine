/**
 * @file script_importer.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Script importer
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <fstream>

#include "managers/importer.h"
#include "script.h"

IMPORTER(Script, "script") {
    auto stream = std::ifstream(path);

    if (!stream.good()) return nullptr;

    return new Asset<Script>(
        std::string(std::istreambuf_iterator<char>{stream}, {}));
}

XML_IMPORTER(Script, "script") {
    const char* path = nullptr;
    data.QueryStringAttribute("path", &path);
    data.QueryStringAttribute("file", &path);

    if (path) return AssetImporter<Script, "script">::import(std::string(path));

    const char* content = nullptr;

    data.QueryStringAttribute("content", &content);
    data.QueryStringAttribute("script", &content);
    data.QueryStringAttribute("code", &content);

    if (!content) {
        log_printf(ERROR_REPORTS, "error",
                   "Neither path nor content of a script were found.\n");

        return nullptr;
    }

    return new Asset<Script>(std::string(content));
}
