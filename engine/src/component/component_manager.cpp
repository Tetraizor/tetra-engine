#include "engine/component/component_manager.h"
#include "engine/component/component.h"
#include "engine/component/component_registry.h"
#include "engine/entity/entity_id.h"

namespace Engine
{
    ComponentManager::ComponentManager(Stage *owner_stage_ptr)
    {
        stage = owner_stage_ptr;

        Logger::log_info("[ComponentManager] Initialization complete");
        Logger::log_info("[ComponentManager] Loaded " + std::to_string(ComponentRegistry::get_instance().get_type_names().size()) + " components.");
    }

    void ComponentManager::destroy_component(const ComponentID id)
    {
        auto it = component_list.find(id);
        if (it == component_list.end())
            return;

        component_destroyed.invoke(it->second);

        component_list.erase(it);

        free_indices.push_back(id.index);
        generations[id.index]++;
    }

    Component *ComponentManager::get_component_by_id(const ComponentID id) const
    {
        auto it = component_list.find(id);
        if (it == component_list.end())
            return nullptr;

        return it->second.get();
    }

    ComponentID ComponentManager::allocate_id()
    {
        uint32_t index;

        if (!free_indices.empty())
        {
            index = free_indices.back();
            free_indices.pop_back();
        }
        else
        {
            index = static_cast<uint32_t>(generations.size());
            generations.push_back(0);
        }

        return {index, generations[index]};
    }

    void ComponentManager::resolve_references()
    {
    }

    void ComponentManager::serialize(Serialization::SerializationContext &ctx) const
    {
        ctx.begin_array_key("components");

        for (const auto &[id, component_ptr] : component_list)
        {
            component_ptr->serialize(ctx);
        }

        ctx.end_array();
    }

    void ComponentManager::deserialize(Serialization::SerializationContext &ctx)
    {
        component_list.clear();
        generations.clear();
        free_indices.clear();

        ctx.begin_array_key("components");

        for (int i = 0; i < ctx.size(); i++)
        {
            ctx.begin_object_index(i);

            std::string component_type = ctx.read<std::string>("type");

            std::shared_ptr<Component> component = ComponentRegistry::get_instance().instantiate_raw(component_type);
            if (!component)
                throw std::runtime_error("Unknown component type: " + component_type);

            component->deserialize(ctx);

            ctx.end_object();
        }

        ctx.end_array();
    }
}
