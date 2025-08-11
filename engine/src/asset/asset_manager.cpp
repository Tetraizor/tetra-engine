#include "engine/asset/asset_manager.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace Engine::Asset
{
    void AssetManager::init(const std::string &asset_root)
    {
        this->asset_root = asset_root;
        scan_assets();
    }

    void AssetManager::scan_assets()
    {
        if (!fs::exists(asset_root))
            throw std::runtime_error("Asset root directory does not exist: " + asset_root);

        if (!fs::is_directory(asset_root))
            throw std::runtime_error("Asset root path is not a directory: " + asset_root);

        for (const auto &entry : fs::recursive_directory_iterator(asset_root))
        {
            if (entry.is_directory())
                continue;

            const std::string asset_path = entry.path().string();

            if (entry.path().extension() == ASSET_META_EXTENSION)
                continue;

            std::string meta_path = asset_path + ASSET_META_EXTENSION;

            AssetMeta asset_meta;
            if (fs::exists(meta_path))
            {
                std::ifstream in(meta_path);
                if (!in.is_open())
                    throw std::runtime_error("Failed to open meta file: " + meta_path);

                std::string meta_json(
                    (std::istreambuf_iterator<char>(in)),
                    (std::istreambuf_iterator<char>()));

                asset_meta.deserialize(meta_json);
            }
            else
            {
                asset_meta.guid = GUID::generate();

                std::error_code ec;
                auto relative_path = fs::relative(entry.path(), asset_root, ec);
                if (ec)
                    throw std::runtime_error("Failed to compute relative path for: " + asset_path);

                asset_meta.path = relative_path.string();

                // TODO: Change this to a better way to handle types
                asset_meta.type = entry.path().extension().string();

                std::ofstream out(meta_path);
                if (!out.is_open())
                    throw std::runtime_error("Failed to create meta file: " + meta_path);

                out << asset_meta.serialize();
            }

            register_asset(asset_meta);
        }
    }

    void AssetManager::register_asset(const AssetMeta &asset_meta)
    {
        path_to_guid[asset_meta.path] = asset_meta.guid;
        guid_to_meta[asset_meta.guid] = asset_meta;
    }
}
