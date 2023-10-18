{
    AssetManager& instance = get_instance();

    auto asset = instance.assets_.find(identifier);
    if (asset != instance.assets_.end()) {
        return &((Asset<T>*)asset->second)->content;
    }

    log_printf(STATUS_REPORTS, "status", "Loading asset %s\n", identifier);

    static char signature[128] = "";

    signature[0] = '\0';

    char* sign_ptr = signature;

    unsigned section_count = 0;

    for (const char* ptr = identifier; *ptr != '\0'; ++ptr) {
        if (*ptr == '.')
            section_count++;
        else if (section_count == 1) {
            *sign_ptr = *ptr;
            ++sign_ptr;
        }
    }

    *sign_ptr = '\0';

    auto importer_cell = instance.importers_.find(signature);
    if (importer_cell == instance.importers_.end()) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to find importer matching signature %s\n",
                   signature);
        return nullptr;
    }

    Asset<T>* imported = (Asset<T>*)importer_cell->second->import(identifier);

    if (imported == nullptr) {
        log_printf(ERROR_REPORTS, "error",
                   "Failed to import asset %s (signature %s)\n", identifier,
                   signature);
        return nullptr;
    }

    return &imported->content;
}