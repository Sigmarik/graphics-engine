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

#pragma once

#include <stdlib.h>
#include <tinyxml2.h>

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "hash/murmur.h"
#include "logger/logger.h"

static const size_t PATH_LENGTH = 1024;

struct ImporterId;
struct AbstractAsset;
struct AbstractImporter;
struct AbstractXMLImporter;

/**
 * @brief Asset/importer registry and dispatcher
 *
 */
struct AssetManager final {
    /**
     * @brief Asset import pipeline modifier
     *
     */
    enum RequestFlag {
        // Do not print any warnings/errors on failure
        Silent = 0x01,
        // Ignore cached assets, import anyways
        Reimport = 0x02,
        // Do not cache the asset
        Rogue = 0x04,
    };

    using RequestFlags = unsigned;

    static void register_importer(AbstractImporter& importer);
    static void register_importer(AbstractXMLImporter& importer);

    /**
     * @brief Request asset from a file
     *
     * @tparam T asset type
     * @param[in] path path to the asset file
     * @param[in] signature optional signature override
     * @param[in] flags
     * @return const T* pointer to the asset, `nullptr` if could not import
     */
    template <typename T>
    static const T* request(const std::string& path,
                            std::optional<std::string_view> signature = {},
                            RequestFlags flags = 0);

    /**
     * @brief Request asset of type T from an XML element
     *
     * @tparam T asset type
     * @param[in] element
     * @param[in] handle optional asset identifier for persistent storage (rogue
     * import if not specified)
     * @param[in] flags
     * @return const T* pointer to the asset, `nullptr` if could not import
     */
    template <typename T>
    static const T* request(tinyxml2::XMLElement& element,
                            std::optional<std::string_view> handle = {},
                            RequestFlags flags = 0);

    /**
     * @brief Clear all cached and rogue assets
     *
     */
    void unload_all();

    /**
     * @brief Register a rogue asset that is not bound to an identifier
     *
     * @param[in] asset
     */
    static void register_rogue(AbstractAsset* asset);

   private:
    AssetManager() = delete;
    AssetManager(const AssetManager& manager) = delete;
    AssetManager& operator=(const AssetManager& manager) = delete;

    static std::string extract_signature(std::string_view name);

    static AbstractImporter* find_importer(const ImporterId& id);
    static AbstractXMLImporter* find_xml_importer(const ImporterId& id);

    struct AssetRequest final {
        AssetRequest(const std::string& asset_path, size_t type)
            : path(asset_path), type_id(type) {}

        std::string path;
        size_t type_id;

        bool operator==(const AssetRequest& request) const;
    };

    friend struct std::hash<AssetManager::AssetRequest>;

    static std::unordered_map<ImporterId, AbstractImporter*> importers_;
    static std::unordered_map<ImporterId, AbstractXMLImporter*> xml_importers_;
    static std::unordered_map<AssetRequest, AbstractAsset*> assets_;
    static std::vector<AbstractAsset*> rogues_;
};

struct AbstractAsset {
    virtual ~AbstractAsset() = default;

    hash_t hash = 0;
};

template <class T>
struct Asset : public AbstractAsset {
    template <class... Ts>
    Asset(Ts&&... args) : content(args...) {}

    T content;
};

struct ImporterId final {
    ImporterId(size_t type, const std::string& sign)
        : type_id(type), signature(sign) {}

    size_t type_id;
    std::string signature;

    bool operator==(const ImporterId& id) const;
};

template <>
struct std::hash<ImporterId> {
    size_t operator()(const ImporterId& id) const noexcept;
};

struct AbstractImporter {
    AbstractImporter(size_t type_id, const std::string& signature);
    virtual ~AbstractImporter() = default;

    virtual AbstractAsset* import(const std::string& path,
                                  AssetManager::RequestFlags flags) const = 0;

    const ImporterId& get_id() const { return id_; }

   private:
    const ImporterId id_;
};

struct AbstractXMLImporter {
    AbstractXMLImporter(size_t type_id, const std::string& signature);

    virtual ~AbstractXMLImporter() = default;
    virtual AbstractAsset* import(tinyxml2::XMLElement& data,
                                  AssetManager::RequestFlags flags) const = 0;

    const ImporterId& get_id() const { return id_; }

   private:
    const ImporterId id_;
};

template <size_t N>
struct StringLiteral final {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    char value[N];
};

template <class ASSET_T, StringLiteral SIGNATURE>
struct AssetImporter : AbstractImporter {
    AssetImporter()
        : AbstractImporter(typeid(ASSET_T).hash_code(), SIGNATURE.value) {}
};

template <class ASSET_T, StringLiteral SIGNATURE>
struct XMLAssetImporter : AbstractXMLImporter {
    XMLAssetImporter()
        : AbstractXMLImporter(typeid(ASSET_T).hash_code(), SIGNATURE.value) {}
};

template <>
struct std::hash<AssetManager::AssetRequest> {
    size_t operator()(const AssetManager::AssetRequest& request) const noexcept;
};

#include "_am_request.hpp"
