#include "asset_manager.h"

#include <string.h>

#include "ctype.h"
#include "logger/logger.h"

void AssetManager::register_importer(AssetImporter& importer,
                                     const char* signature) {
    AssetManager& instance = get_instance();

    if (instance.importers_.find(signature) != instance.importers_.end()) {
        log_printf(ERROR_REPORTS, "error",
                   "Asset importer for signature \"%s\" has already been "
                   "registered.\n",
                   signature);
        return;
    }

    instance.importers_.insert({signature, &importer});
}

void AssetManager::unload(const char* identifier) {
    AssetManager& instance = get_instance();

    auto cell = instance.assets_.find(identifier);

    if (cell == instance.assets_.end()) return;

    instance.assets_.erase(cell);
}

AssetManager& AssetManager::get_instance() {
    static AssetManager manager;
    return manager;
}

AssetManager::~AssetManager() {
    for (auto cell : assets_) {
        delete cell.second;
    }

    assets_.clear();
}

const char* trim_path(const char* path) {
    static char trimmed[PATH_LENGTH] = "";

    copy_trimmed(trimmed, path);

    return trimmed;
}

void copy_trimmed(char destination[PATH_LENGTH], const char* source) {
    char* out = destination;

    for (const char* chr = source; *chr != '\0'; ++chr) {
        if (!isgraph(*chr)) continue;

        *out = *chr;
        ++out;
    }

    *out = '\0';
}
