#include "component_registry.h"

#include <cassert>

namespace Engine
{
    ComponentRegistry &ComponentRegistry::instance()
    {
        static ComponentRegistry registry;
        return registry;
    }

    std::unique_ptr<Component> ComponentRegistry::instantiate_raw(const std::string &name) const
    {
        auto it = creators.find(name);
        if (it != creators.end())
        {
            return it->second();
        }

        throw std::runtime_error("Could not find Component with type: " + name);
    }

    std::string ComponentRegistry::get_name(const Component *component) const
    {
        if (!component)
            return {};

        std::type_index type_id(typeid(*component));

        auto it = type_names.find(type_id);
        if (it != type_names.end())
        {
            return it->second;
        }
        return {};
    }
}