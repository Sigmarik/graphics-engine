#include "asset_manager.h"

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
