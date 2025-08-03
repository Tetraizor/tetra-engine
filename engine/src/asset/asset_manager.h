#pragma once

#include <string>
#include "base/singleton.h"
#include "asset/asset_meta.h"
#include "asset/asset_base.h"

// TODO: Implement a virtual file system for exports

namespace Engine::Asset
{
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        static inline const std::string ASSET_META_EXTENSION = ".assetmeta";

        AssetManager() = default;
        ~AssetManager() = default;

        void init(const std::string &asset_root);

        template <typename T>
        std::shared_ptr<T> load(const std::string &path);

        template <typename T>
        std::shared_ptr<T> load_by_guid(const GUID &guid);

        const AssetMeta *get_meta_by_path(const std::string &path) const;
        const AssetMeta *get_meta_by_guid(const GUID &guid) const;

    private:
        std::string asset_root;

        std::unordered_map<GUID, AssetMeta> guid_to_meta;

        std::unordered_map<std::string, GUID> path_to_guid;
        std::unordered_map<GUID, std::shared_ptr<AssetBase>> loaded_assets;

        void scan_assets();
        void register_asset(const AssetMeta &asset_meta);
    };

    template <typename T>
    std::shared_ptr<T> AssetManager::load(const std::string &path)
    {
        auto it = path_to_guid.find(path);
        if (it == path_to_guid.end())
            return nullptr;

        return load_by_guid<T>(it->second);
    }

    template <typename T>
    std::shared_ptr<T> AssetManager::load_by_guid(const GUID &guid)
    {
        static_assert(std::is_base_of<AssetBase, T>::value, "T must derive from AssetBase");

        auto it = loaded_asset.find(guid);
        if (it != loaded_assets.end())
            return std::dynamic_pointer_cast<T>(it->second.data);

        auto meta_it = guid_to_meta.find(guid);
        if (meta_it == guid_to_meta.end())
            return nullptr;

        const AssetMeta &meta = meta_it->second;

        std::shared_ptr<T> asset_ptr = std::make_shared<T>();

        asset_ptr->set_meta(meta);
        asset_ptr->load();

        loaded_assets[guid] = asset_ptr;
        return asset_ptr;
    }
}