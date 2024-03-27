/**
 * @file importer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Abstract importer header, intended for use in `importer.cpp` files
 * only
 * @version 0.1
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "asset_manager.h"

/**
 * @brief Create and register an asset importer
 *
 */
#define IMPORTER(type, signature)                                             \
    template <>                                                               \
    struct AssetImporter<type, signature> : AbstractImporter {                \
        AssetImporter()                                                       \
            : AbstractImporter(typeid(type).hash_code(), signature) {}        \
                                                                              \
        AbstractAsset* import(                                                \
            const std::string& path,                                          \
            AssetManager::RequestFlags flags) const override;                 \
                                                                              \
        static AssetImporter<type, signature> instance_;                      \
    };                                                                        \
                                                                              \
    AssetImporter<type, signature> AssetImporter<type, signature>::instance_; \
                                                                              \
    AbstractAsset* AssetImporter<type, signature>::import(                    \
        const std::string& path, AssetManager::RequestFlags flags) const

/**
 * @brief Create and register an XML asset importer
 *
 */
#define XML_IMPORTER(type, signature)                                       \
    template <>                                                             \
    struct XMLAssetImporter<type, signature> : AbstractXMLImporter {        \
        AssetXMLImporter()                                                  \
            : AbstractXMLImporter(typeid(type).hash_code(), signature) {}   \
                                                                            \
        AbstractAsset* import(                                              \
            const tinyxml2::XMLElement& data,                               \
            AssetManager::RequestFlags flags) const override;               \
                                                                            \
        static XMLAssetImporter<type, signature> instance_;                 \
    };                                                                      \
                                                                            \
    XMLAssetImporter<type, signature>                                       \
        XMLAssetImporter<type, signature>::instance_;                       \
                                                                            \
    AbstractAsset* XMLAssetImporter<type, signature>::import(               \
        const tinyxml2::XMLElement& data, AssetManager::RequestFlags flags) \
        const

#define WARNING(...)                                            \
    do {                                                        \
        if ((flags & AssetManager::RequestFlag::Silent) == 0) { \
            log_printf(WARNINGS, "warning", __VA_ARGS__);       \
        }                                                       \
    } while (0)

#define ERROR(...)                                              \
    do {                                                        \
        if ((flags & AssetManager::RequestFlag::Silent) == 0) { \
            log_printf(ERROR_REPORTS, "error", __VA_ARGS__);    \
        }                                                       \
    } while (0)
