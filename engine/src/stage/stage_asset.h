#pragma once

#include "asset/asset_base.h"
#include "asset/asset_type_registry.h"
#include "stage/stage.h"
#include "utils/io.h"
#include "asset/asset_meta.h"
#include "serialization/json/json_document.h"
#include "serialization/json/json_value.h"
#include "serialization/json/json_serialization_context.h"

using namespace Engine::Serialization::Json;

namespace Engine::Asset
{
    class StageAsset : public AssetBase
    {
    public:
        StageAsset() {}

        void load() override
        {
        }

        void unload() override
        {
        }

        std::string get_type_name() const
        {
            return "Stage";
        }

        std::shared_ptr<Engine::RuntimeObjectBase> instantiate() const
        {
            auto stage_ptr = std::make_shared<Stage>();

            auto content_opt = Utils::IO::read_file_contents(get_meta().path);
            if (!content_opt.has_value())
            {
                // log error or fallback behavior
                return nullptr;
            }
            JsonDocument doc(content_opt.value());
            JSONSerializationContext ctx(stage_ptr.get(), doc);

            stage_ptr->deserialize(ctx);

            return stage_ptr;
        }
    };
}

REGISTER_ASSET_TYPE(Engine::Asset::StageAsset, "Stage")
