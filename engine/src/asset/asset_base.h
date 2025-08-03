#pragma once

#include <string>

#include "serialization/serializable.h"
#include "base/runtime_object_base.h"

using namespace Engine::Serialization;

namespace Engine::Asset
{
    enum class AssetLoadState
    {
        Unloaded,
        Loading,
        Loaded,
        Failed
    };

    class AssetBase
    {
    public:
        virtual ~AssetBase() = default;

        virtual void load() = 0;
        virtual void unload() = 0;

        virtual AssetLoadState get_load_state() const { return load_state; };

        virtual std::string get_type_name() const = 0;

        const AssetMeta &get_meta() const { return meta; }
        void set_meta(const AssetMeta &meta) { this->meta = meta; }

        virtual std::shared_ptr<Engine::RuntimeObjectBase> instantiate() const = 0;

        template <typename T>
        std::shared_ptr<T> instantiate_typed() const
        {
            static_assert(std::is_base_of<Engine::RuntimeObjectBase, T>::value, "T must derive from RuntimeObjectBase");
            return std::dynamic_pointer_cast<T>(instantiate());
        }

    protected:
        AssetMeta meta;

        AssetLoadState load_state;
    };
}