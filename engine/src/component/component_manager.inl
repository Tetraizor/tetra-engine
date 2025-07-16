#pragma once

#include "stage/stage.h"
#include "entity/entity_manager.h"

namespace Engine
{
    template <typename T>
    Component *ComponentManager::create_component(EntityID owner_id)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

        Entity *entity = stage->get_entity_manager().get_entity_by_id(owner_id);

        if (!entity)
        {
            throw std::runtime_error("Cannot find entity with provided EntityID!");
        }

        ComponentID component_id = allocate_id();

        std::unique_ptr<T> component_ptr = ComponentRegistry::instance().create<T>();

        component_ptr->id = id;
        component_ptr->entity = entity;

        component_list[id] = std::move(component_ptr);
        return component_list[id].get();
    }
}
