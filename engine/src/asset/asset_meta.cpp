#include "asset_meta.h"

#include "serialization/json/json_value.h"
#include "serialization/json/json_document.h"

using namespace Engine::Serialization::Json;

namespace Engine::Asset
{
    void AssetMeta::deserialize(const std::string &asset_meta_json)
    {
        JsonDocument doc(asset_meta_json);

        JsonValue root = doc.get_root();
        guid = GUID::from_string(root.get("guid").as<std::string>().value());
        path = root.get("path").as<std::string>().value();
        type = root.get("type").as<std::string>().value();
    }

    std::string AssetMeta::serialize() const
    {
        JsonDocument doc;

        JsonValue root = doc.get_root();
        root.set("guid", guid.to_string());
        root.set("path", path);
        root.set("type", type);

        return doc.to_text(true);
    }
}