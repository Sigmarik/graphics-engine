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
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "hash/murmur.h"
#include "logger/logger.h"

static const size_t PATH_LENGTH = 1024;

const char* trim_path(const char* path);
void copy_trimmed(char destination[PATH_LENGTH], const char* source);

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

struct ImporterId {
    ImporterId(size_t type, const char* sign)
        : type_id(type), signature(sign) {}

    size_t type_id;
    const char* signature;

    bool operator==(const ImporterId& id) const;
};

struct AbstractImporter {
    AbstractImporter(size_t type_id, const char* signature);

    virtual ~AbstractImporter() = default;
    virtual AbstractAsset* import(const char* path) const = 0;

    const ImporterId& get_id() const { return id_; }

   private:
    const ImporterId id_;
};

template <class ASSET_T, const char* SIGNATURE>
struct AssetImporter : AbstractImporter {
    AssetImporter()
        : AbstractImporter(typeid(ASSET_T).hash_code(), SIGNATURE) {}
};

struct AssetManager {
    static void register_importer(AbstractImporter& importer);

    template <typename T>
    static const T* request(const char* path);

    void unload_all();

   private:
    AssetManager() = delete;
    AssetManager(const AssetManager& manager) = delete;
    AssetManager& operator=(const AssetManager& manager) = delete;

    struct AssetRequest {
        AssetRequest(const char* asset_path, size_t type)
            : path(asset_path), type_id(type) {}

        const char* path = nullptr;
        size_t type_id = 0;

        bool operator==(const AssetRequest& request) const;
    };

    friend struct std::hash<AssetManager::AssetRequest>;

    static std::unordered_map<ImporterId, AbstractImporter*> importers_;
    static std::unordered_map<AssetRequest, AbstractAsset*> assets_;
};

template <>
struct std::hash<ImporterId> {
    size_t operator()(const ImporterId& id) const noexcept;
};

template <>
struct std::hash<AssetManager::AssetRequest> {
    size_t operator()(const AssetManager::AssetRequest& request) const noexcept;
};

#include "_am_request.hpp"

/**
 * @brief Create and register asset importer (for .cpp importer lists)
 *
 */
#define IMPORTER(type, signature)                                         \
    const char __string_##signature[] = #signature;                       \
                                                                          \
    template <>                                                           \
    struct AssetImporter<type, __string_##signature> : AbstractImporter { \
        AssetImporter()                                                   \
            : AbstractImporter(typeid(type).hash_code(),                  \
                               __string_##signature) {}                   \
                                                                          \
        AbstractAsset* import(const char* path) const override;           \
                                                                          \
        static AssetImporter<type, __string_##signature> instance_;       \
    };                                                                    \
                                                                          \
    AssetImporter<type, __string_##signature>                             \
        AssetImporter<type, __string_##signature>::instance_;             \
                                                                          \
    AbstractAsset* AssetImporter<type, __string_##signature>::import(     \
        const char* path) const

#endif
