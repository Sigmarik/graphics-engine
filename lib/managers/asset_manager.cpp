#include "asset_manager.h"

#include <string.h>

#include <functional>

#include "ctype.h"
#include "logger/logger.h"

size_t std::hash<ImporterId>::operator()(const ImporterId& id) const noexcept {
    size_t left = id.type_id;
    size_t right = std::hash<std::string>{}(id.signature);
    return left ^ right;
}

size_t std::hash<AssetManager::AssetRequest>::operator()(
    const AssetManager::AssetRequest& request) const noexcept {
    size_t left = request.type_id;
    size_t right = std::hash<std::string>{}(request.path);
    return left ^ right;
}

std::unordered_map<ImporterId, AbstractImporter*> AssetManager::importers_ = {};
std::unordered_map<AssetManager::AssetRequest, AbstractAsset*>
    AssetManager::assets_ = {};

void AssetManager::register_importer(AbstractImporter& importer) {
    const ImporterId& id = importer.get_id();

    if (importers_.find(id) != importers_.end()) {
        log_printf(
            ERROR_REPORTS, "error",
            "Asset importer of type %0lX for signature \"%s\" has already been "
            "registered.\n",
            id.type_id, id.signature);
        return;
    }

    importers_.insert({id, &importer});
}

void AssetManager::unload_all() {
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

AbstractImporter::AbstractImporter(size_t type_id, const char* signature)
    : id_(type_id, signature) {
    AssetManager::register_importer(*this);
}

bool AssetManager::AssetRequest::operator==(const AssetRequest& request) const {
    if (request.type_id != type_id) return false;
    if (request.path == path) return true;

    return strcmp(request.path, path) == 0;
}

bool ImporterId::operator==(const ImporterId& id) const {
    if (id.type_id != type_id) return false;
    if (id.signature == signature) return true;

    return strcmp(id.signature, signature) == 0;
}
