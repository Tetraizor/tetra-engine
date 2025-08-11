#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <stdexcept>

#include "engine/base/singleton.h"
#include "engine/asset/asset_base.h"

namespace Engine::Asset
{
    class AssetTypeRegistry : public Singleton<AssetTypeRegistry>
    {
    public:
        using FactoryFunction = std::function<std::shared_ptr<AssetBase>()>;

        void register_type(const std::string &type_name, FactoryFunction factory)
        {
            if (registry.find(type_name) != registry.end())
            {
                throw std::runtime_error("Asset type \"" + type_name + "\" is already registered");
            }

            registry[type_name] = std::move(factory);
        }

        std::shared_ptr<AssetBase> create(const std::string &type_name) const
        {
            auto it = registry.find(type_name);
            if (it == registry.end())
            {
                throw std::runtime_error("Asset type \"" + type_name + "\" is not registered");
            }

            return it->second();
        }

        bool is_registered(const std::string &type_name) const
        {
            return registry.find(type_name) != registry.end();
        }

    private:
        std::unordered_map<std::string, FactoryFunction> registry;
    };
}

#pragma once

#define REGISTER_ASSET_TYPE(AssetClassName, TypeNameStr)                                 \
    namespace                                                                            \
    {                                                                                    \
        struct AssetRegister_##__COUNTER__                                               \
        {                                                                                \
            AssetRegister_##__COUNTER__()                                                \
            {                                                                            \
                Engine::Asset::AssetTypeRegistry::get_instance().register_type(          \
                    TypeNameStr,                                                         \
                    []() -> std::shared_ptr<Engine::Asset::AssetBase>                    \
                    {                                                                    \
                        return std::make_shared<AssetClassName>();                       \
                    });                                                                  \
            }                                                                            \
        };                                                                               \
        static AssetRegister_##__COUNTER__ global_asset_register_instance_##__COUNTER__; \
    }
