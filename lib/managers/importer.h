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
 * @brief Importer declarations
 *
 * @warning Does not include definitions
 * @see IMPORTER
 *
 */
#define IMPORTER_HEAD(type, signature)                                       \
    template <>                                                              \
    struct AssetImporter<type, signature> : AbstractImporter {               \
        AssetImporter()                                                      \
            : AbstractImporter(typeid(type).hash_code(), signature) {}       \
                                                                             \
        AbstractAsset* local_import(                                         \
            const std::string& path,                                         \
            AssetManager::RequestFlags flags) const override;                \
                                                                             \
        static AbstractAsset* import(const std::string& path,                \
                                     AssetManager::RequestFlags flags = 0) { \
            return instance_.local_import(path, flags);                      \
        }                                                                    \
                                                                             \
        static AssetImporter<type, signature> instance_;                     \
    };

/**
 * @brief Importer's `import` method definition header
 *
 * @warning Does not include declarations
 * @see IMPORTER
 *
 */
#define IMPORTER_TAIL(type, signature)                                        \
    AssetImporter<type, signature> AssetImporter<type, signature>::instance_; \
                                                                              \
    AbstractAsset* AssetImporter<type, signature>::local_import(              \
        const std::string& path, AssetManager::RequestFlags flags) const

/**
 * @brief Create and register an asset importer
 *
 */
#define IMPORTER(type, signature)  \
    IMPORTER_HEAD(type, signature) \
    IMPORTER_TAIL(type, signature)

/**
 * @brief XML importer declarations
 *
 * @warning Does not include `import` method definition
 * @see XML_IMPORTER
 *
 */
#define XML_IMPORTER_HEAD(type, signature)                                   \
    template <>                                                              \
    struct XMLAssetImporter<type, signature> : AbstractXMLImporter {         \
        XMLAssetImporter()                                                   \
            : AbstractXMLImporter(typeid(type).hash_code(), signature) {}    \
                                                                             \
        AbstractAsset* local_import(                                         \
            const tinyxml2::XMLElement& data,                                \
            AssetManager::RequestFlags flags) const override;                \
                                                                             \
        static AbstractAsset* import(const tinyxml2::XMLElement& data,       \
                                     AssetManager::RequestFlags flags = 0) { \
            return instance_.local_import(data, flags);                      \
        }                                                                    \
                                                                             \
        static XMLAssetImporter<type, signature> instance_;                  \
    };

/**
 * @brief XML importer's `import` method definition header
 *
 * @warning Does not include declarations
 * @see XML_IMPORTER
 *
 */
#define XML_IMPORTER_TAIL(type, signature)                                  \
    XMLAssetImporter<type, signature>                                       \
        XMLAssetImporter<type, signature>::instance_;                       \
                                                                            \
    AbstractAsset* XMLAssetImporter<type, signature>::local_import(         \
        const tinyxml2::XMLElement& data, AssetManager::RequestFlags flags) \
        const

/**
 * @brief Create and register an XML asset importer
 *
 */
#define XML_IMPORTER(type, signature)  \
    XML_IMPORTER_HEAD(type, signature) \
    XML_IMPORTER_TAIL(type, signature)

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

/**
 * @brief Create and register an XML importer with regular importer mirror
 *
 */
#define XML_BASED_IMPORTER(type, signature)                                  \
    XML_IMPORTER_HEAD(type, signature)                                       \
                                                                             \
    IMPORTER(type, signature) {                                              \
        tinyxml2::XMLDocument doc;                                           \
        doc.LoadFile(path.c_str());                                          \
                                                                             \
        const tinyxml2::XMLElement* data = doc.FirstChildElement(signature); \
                                                                             \
        if (data == nullptr) {                                               \
            ERROR("Could not find the \"" signature "\" tag in \"%s\"\n",    \
                  path.c_str());                                             \
            return nullptr;                                                  \
        }                                                                    \
                                                                             \
        return XMLAssetImporter<type, signature>::import(*data, flags);      \
    }                                                                        \
                                                                             \
    XML_IMPORTER_TAIL(type, signature)
