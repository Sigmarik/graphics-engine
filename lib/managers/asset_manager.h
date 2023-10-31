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

const char* trim_path(const char* path);

struct AbstractAsset {
    virtual ~AbstractAsset() = default;

    hash_t hash = 0;
};

template <typename T>
struct Asset : public AbstractAsset {
    template <typename... Ts>
    Asset<T>(Ts&&... args) : content(args...) {}

    T content;
};

struct AssetImporter {
    virtual ~AssetImporter() = default;
    virtual AbstractAsset* import(const char* path) = 0;
};

struct AssetManager {
    static void register_importer(AssetImporter& importer,
                                  const char* signature);

    template <typename T>
    static T* request(const char* identifier) {
#include "_am_request.hpp"
    }

    static void unload(const char* identifier);

   private:
    static AssetManager& get_instance();

    ~AssetManager();

    AssetManager() = default;
    AssetManager(const AssetManager& manager) = default;
    AssetManager& operator=(const AssetManager& manager) = default;

    std::unordered_map<std::string, AssetImporter*> importers_ = {};
    std::unordered_map<std::string, AbstractAsset*> assets_ = {};
};

/**
 * @brief Create and register asset importer
 *
 */
#define IMPORTER(name)                                    \
    struct name : AssetImporter {                         \
        AbstractAsset* import(const char* path) override; \
    };                                                    \
    __attribute__((constructor)) void __register##name();

/**
 * @brief Register asset importer
 *
 */
#define REGISTER(importer, signature)                                         \
    __attribute__((constructor)) void __register##importer##__##signature() { \
        static importer imp;                                                  \
                                                                              \
        static bool initialized = false;                                      \
                                                                              \
        if (initialized) return;                                              \
                                                                              \
        initialized = true;                                                   \
                                                                              \
        log_printf(STATUS_REPORTS, "status",                                  \
                   "Registering asset importer " #importer                    \
                   " to signature " #signature ".\n");                        \
                                                                              \
        AssetManager::register_importer(imp, #signature);                     \
    }

#endif
