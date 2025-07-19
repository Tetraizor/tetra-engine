#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>

#include "stage/stage.h"
#include "component/component_id.h"
#include "component/component.h"
#include "entity/entity_id.h"
#include "serialization/serializable.h"

namespace Engine
{
    class Component;

    class ComponentManager : public Serializable
    {
        friend class Stage;

    public:
        ComponentManager(Stage *owner_stage_ptr);
        ~ComponentManager() = default;

        template <typename T>
        Component *create_component(EntityID owner_id);

        void destroy_component(const ComponentID id);
        Component *get_component_by_id(const ComponentID id) const;

        void serialize(SerializationContext &ctx) const override;
        void deserialize(SerializationContext &ctx) override;

        ComponentID allocate_id();

    private:
        std::unordered_map<ComponentID, std::unique_ptr<Component>> component_list;

        std::vector<uint32_t> generations;
        std::vector<uint32_t> free_indices;

        Stage *stage = nullptr;
    };
}

#include "component/component_manager.inl"