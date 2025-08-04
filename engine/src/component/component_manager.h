#pragma once

#include "stage/stage.h"
#include "component/component_id.h"
#include "component/component.h"
#include "entity/entity_id.h"
#include "serialization/serializable.h"

#include "base/event.h"

#include <iostream>
#include <unordered_map>
#include <memory>

namespace Engine
{
    class Component;

    class ComponentManager : public Serialization::Serializable
    {
        friend class Stage;

    public:
        ComponentManager(Stage *owner_stage_ptr);
        ~ComponentManager() = default;

        Event<> component_created;
        Event<> component_destroyed;

        template <typename T>
        std::shared_ptr<Component> create_component(EntityID owner_id);

        void destroy_component(const ComponentID id);
        Component *get_component_by_id(const ComponentID id) const;

        void resolve_references();

        void serialize(Serialization::SerializationContext &ctx) const override;
        void deserialize(Serialization::SerializationContext &ctx) override;

        ComponentID allocate_id();

    private:
        std::unordered_map<ComponentID, std::shared_ptr<Component>> component_list;

        std::vector<uint32_t> generations;
        std::vector<uint32_t> free_indices;

        Stage *stage = nullptr;
    };
}

#include "component/component_manager.inl"