#include "asset_manager.h"

#include "logger/logger.h"

AssetTypeId AssetManager::register_importer(AssetImporter& importer) {
    AssetManager& instance = get_instance();

    instance.importers_.push_back(&importer);

    return (AssetTypeId)instance.importers_.size() - 1;
}

AbstractAsset* AssetManager::request(const char* identifier,
                                     AssetTypeId type_id) {
    AssetManager& instance = get_instance();

    auto asset = instance.assets_.find(identifier);
    if (asset != instance.assets_.end()) {
        return asset->second;
    }

    log_printf(STATUS_REPORTS, "status", "Loading asset %s (type id %u).\n",
               identifier, type_id);

    AbstractAsset* loaded = instance.importers_[type_id]->import(identifier);

    if (loaded == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to load asset %s (type id %u).\n", identifier,
                   type_id);
        return nullptr;
    }

    instance.assets_.insert({identifier, loaded});

    return loaded;
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
