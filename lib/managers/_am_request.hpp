#include <string.h>

template <typename T>
const T* AssetManager::request(const std::string& path,
                               std::optional<std::string_view> sign_suggestion,
                               RequestFlags flags) {
    std::string signature = extract_signature(path);

    std::string id_path = path;
    if (sign_suggestion && signature != *sign_suggestion) {
        id_path = path + "\t//." + std::string(*sign_suggestion) + ".//";
    }

    AssetManager::AssetRequest identifier(id_path, typeid(T).hash_code());

    if ((flags & RequestFlag::Reimport) == 0) {
        auto asset = assets_.find(identifier);
        if (asset != assets_.end()) {
            return &((Asset<T>*)asset->second)->content;
        }
    }

    log_printf(STATUS_REPORTS, "status", "Loading asset \"%s\" (type %0lX)\n",
               identifier.path.c_str(), identifier.type_id);

    ImporterId importer_id(typeid(T).hash_code(), signature);

    AbstractImporter* importer = find_importer(importer_id);

    if (importer == nullptr && sign_suggestion) {
        signature = *sign_suggestion;
        importer_id = ImporterId(typeid(T).hash_code(), signature);
        importer = find_importer(importer_id);
    }

    if (importer == nullptr) {
        if ((flags & RequestFlag::Silent) == 0) {
            log_printf(
                ERROR_REPORTS, "error",
                "Failed to find an importer matching the signature \"%s\" "
                "(type %0lX)\n",
                signature.c_str(), importer_id.type_id);
            printf(
                "ERROR: Failed to find an importer for asset \"%s\", see logs "
                "for "
                "more information.\n",
                path.c_str());
        }

        return nullptr;
    }

    Asset<T>* imported = (Asset<T>*)importer->local_import(path, flags);

    if (imported == nullptr) {
        if ((flags & RequestFlag::Silent) == 0) {
            log_printf(ERROR_REPORTS, "error",
                       "Failed to import asset \"%s\" (type %0lX)\n",
                       path.c_str(), identifier.type_id);
            printf(
                "ERROR: Failed to import \"%s\", see logs for more "
                "information.\n",
                path.c_str());
        }
        return nullptr;
    }

    if (flags & RequestFlag::Rogue) {
        register_rogue(imported);
    } else {
        if (assets_.count(identifier) > 0) {
            delete assets_[identifier];
        }

        assets_[identifier] = imported;
    }

    return &imported->content;
}

template <typename T>
const T* AssetManager::request(const tinyxml2::XMLElement& element,
                               std::optional<std::string_view> handle,
                               RequestFlags flags) {
    const char* tag = element.Name();

    std::optional<AssetManager::AssetRequest> identifier{};

    if (handle) {
        identifier = AssetManager::AssetRequest(std::string(*handle),
                                                typeid(T).hash_code());
    }

    if (identifier && (flags & RequestFlag::Reimport) == 0) {
        auto asset = assets_.find(*identifier);
        if (asset != assets_.end()) {
            return &((Asset<T>*)asset->second)->content;
        }
    }

    ImporterId importer_id(typeid(T).hash_code(), tag);
    AbstractXMLImporter* importer = find_xml_importer(importer_id);

    if (importer == nullptr) {
        if ((flags & RequestFlag::Silent) == 0) {
            log_printf(ERROR_REPORTS, "error",
                       "Failed to find importer matching the signature \"%s\" "
                       "(type %0lX)\n",
                       tag, importer_id.type_id);
            printf(
                "ERROR: Failed to find importer for xml asset with tag \"%s\", "
                "see logs for more information.\n",
                tag);
        }

        return nullptr;
    }

    Asset<T>* imported = (Asset<T>*)importer->local_import(element, flags);

    if (imported == nullptr) {
        if ((flags & RequestFlag::Silent) == 0) {
            log_printf(ERROR_REPORTS, "error",
                       "Failed to import XML asset (tag: \"%s\", type %0lX)\n",
                       tag, identifier->type_id);
            printf(
                "ERROR: Failed to import XML asset with tag \"%s\", see logs "
                "for more information.\n",
                tag);
        }
        return nullptr;
    }

    if (!identifier || (flags & RequestFlag::Rogue)) {
        register_rogue(imported);
    } else {
        if (assets_.count(*identifier) > 0) {
            delete assets_[*identifier];
        }

        assets_[*identifier] = imported;
    }

    return &imported->content;
}
