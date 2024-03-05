#include <string.h>

template <typename T>
const T* AssetManager::request(const char* path) {
    AssetManager::AssetRequest identifier(path, typeid(T).hash_code());

    auto asset = assets_.find(identifier);
    if (asset != assets_.end()) {
        return &((Asset<T>*)asset->second)->content;
    }

    log_printf(STATUS_REPORTS, "status", "Loading asset \"%s\" (type %0lX)\n",
               identifier.path, identifier.type_id);

    static char signature[128] = "";

    signature[0] = '\0';

    char* sign_ptr = signature;

    unsigned section_count = 0;

    for (const char* ptr = identifier.path; *ptr != '\0'; ++ptr) {
        if (*ptr == '.')
            section_count++;
        else if (section_count == 1) {
            *sign_ptr = *ptr;
            ++sign_ptr;
        }
    }

    *sign_ptr = '\0';

    ImporterId importer_id(typeid(T).hash_code(), signature);
    auto importer_cell = importers_.find(importer_id);
    if (importer_cell == importers_.end()) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to find importer matching the signature \"%s\" "
                   "(type %0lX)\n",
                   signature, importer_id.type_id);
        return nullptr;
    }

    Asset<T>* imported = (Asset<T>*)importer_cell->second->import(path);

    if (imported == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to import the asset \"%s\" (type %0lX)\n", path,
                   identifier.type_id);
        return nullptr;
    }

    assets_.insert(
        {AssetManager::AssetRequest(strdup(path), typeid(T).hash_code()),
         imported});

    return &imported->content;
}