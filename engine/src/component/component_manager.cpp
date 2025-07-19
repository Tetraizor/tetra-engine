#include "component/component_manager.h"
#include "component/component.h"
#include "component/component_registry.h"
#include "entity/entity_id.h"

namespace Engine
{
    ComponentManager::ComponentManager(Stage *owner_stage_ptr)
    {
        stage = owner_stage_ptr;

        std::cout << "[ComponentManager] Initialized successfully!" << std::endl;
        std::cout << "[ComponentManager] Loaded " << ComponentRegistry::instance().get_type_names().size() << " components:" << std::endl;
    }

    void ComponentManager::destroy_component(const ComponentID id)
    {
        auto it = component_list.find(id);
        if (it == component_list.end())
            return;

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

    void ComponentManager::serialize(SerializationContext &ctx) const
    {
        ctx.begin_array("components");

        for (const auto &[id, component_ptr] : component_list)
        {
            component_ptr->serialize(ctx);
        }

        ctx.end_array();
    }

    void ComponentManager::deserialize(SerializationContext &ctx)
    {
        ctx.begin_array("components");

        for (int i = 0; i < ctx.array_size(); i++)
        {
            ctx.begin_object("component_id");

            EntityID owner_id;
            owner_id.serialize(ctx);

            ctx.end_object();

            std::string component_type = ctx.read_string("component_type");

            std::unique_ptr<Component> component = ComponentRegistry::instance().create(component_type);
            if (!component)
                throw std::runtime_error("Unknown component type: " + component_type);

            component->deserialize(ctx);
        }

        ctx.end_array();
    }
}