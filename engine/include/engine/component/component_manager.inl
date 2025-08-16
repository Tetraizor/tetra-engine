#pragma once

#include <memory>

#include "engine/component/component.h"
#include "engine/entity/entity_manager.h"
#include "engine/entity/entity.h"
#include "engine/component/component_registry.h"
#include "engine/stage/stage_manager.h"

namespace Engine
{
    template <typename T>
    std::shared_ptr<Component> ComponentManager::create_component(EntityID owner_id)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

        Entity *entity = stage->get_entity_manager().get_entity_by_id(owner_id);

        if (!entity)
        {
            throw std::runtime_error("Cannot find entity with provided EntityID!");
        }

        ComponentID component_id = allocate_id();

        ComponentRegistry &component_registry = ComponentRegistry::get_instance();
        std::shared_ptr<T> component_ptr = component_registry.instantiate_raw_typed<T>();

        component_ptr->id = component_id;
        component_ptr->entity = entity;

        component_list[component_id] = component_ptr;

        component_created.invoke(component_ptr);

        return component_list[component_id];
    }
}
