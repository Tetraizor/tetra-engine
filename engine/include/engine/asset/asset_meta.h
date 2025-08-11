#pragma once

#include "engine/data/guid.h"

namespace Engine::Asset
{
    struct AssetMeta
    {
        GUID guid;
        std::string path;
        std::string type;

        std::string serialize() const;
        void deserialize(const std::string &asset_meta_json);
    };
}