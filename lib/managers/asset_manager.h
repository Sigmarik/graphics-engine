/**
 * @file asset_manager.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Asset manager class
 * @version 0.1
 * @date 2023-10-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <stdlib.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "hash/murmur.h"
#include "logger/logger.h"

struct AbstractAsset {
    virtual ~AbstractAsset() = default;

    hash_t hash = 0;
};

typedef unsigned AssetTypeId;
static const AssetTypeId ASSET_TYPE_POISON = (AssetTypeId)-1;

struct AssetImporter {
    virtual ~AssetImporter() = default;
    virtual AbstractAsset* import(const char* path) = 0;
};

struct AssetManager {
    [[nodiscard("ASSET TYPE IDENTIFIER IGNORED")]] static AssetTypeId
    register_importer(AssetImporter& importer);

    static AbstractAsset* request(const char* identifier, AssetTypeId type_id);

    static void unload(const char* identifier);

   private:
    static AssetManager& get_instance();

    ~AssetManager();

    AssetManager() = default;
    AssetManager(const AssetManager& manager) = default;
    AssetManager& operator=(const AssetManager& manager) = default;

    std::vector<AssetImporter*> importers_ = {};
    std::unordered_map<std::string, AbstractAsset*> assets_ = {};
};

/**
 * @brief Register asset importer
 *
 */
#define REGISTER(importer)                                                   \
    AssetTypeId importer::_type_id = ASSET_TYPE_POISON;                      \
    __attribute__((constructor)) void __register##importer() {               \
        static importer imp;                                                 \
                                                                             \
        static bool initialized = false;                                     \
                                                                             \
        if (initialized) return;                                             \
                                                                             \
        initialized = true;                                                  \
                                                                             \
        importer::_type_id = AssetManager::register_importer(imp);           \
                                                                             \
        log_printf(STATUS_REPORTS, "status",                                 \
                   "Asset importer " #importer " loaded with type id %u.\n", \
                   importer::_type_id);                                      \
    }

/**
 * @brief Create and register asset importer
 *
 */
#define IMPORTER(name)                                        \
    struct name : AssetImporter {                             \
        AbstractAsset* import(const char* path) override;     \
        static AssetTypeId get_type_id() { return _type_id; } \
        static AssetTypeId _type_id;                          \
    };                                                        \
    __attribute__((constructor)) void __register##name();

// TODO: Think of a way to store and synch type identifiers.
// Pseudo-members? Dumb yet effective.
//  AssetTypeId _TextureImporter_type_id;
//  TextureImporter {
//      static AssetTypeId get_asset_type() { return _TextureImporter_type_id; }
// }

#endif
