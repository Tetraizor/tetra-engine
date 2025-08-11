#pragma once

#include "engine/engine.h"

#include "engine/serialization/serializable.h"

#include <iostream>
#include <unordered_map>
#include <memory>

namespace Engine
{
    class Component;
    class ComponentRegistry;

    class ComponentManager : public Serialization::Serializable
    {
        friend class Stage;

    public:
        ComponentManager(Stage *owner_stage_ptr);
        ~ComponentManager() = default;

        Event<std::weak_ptr<Component>> component_created;
        Event<std::weak_ptr<Component>> component_destroyed;

        template <typename T>
        std::shared_ptr<Component> create_component(EntityID owner_id);

        void destroy_component(const ComponentID id);
        Component *get_component_by_id(const ComponentID id) const;

        template <typename T>
        std::vector<std::weak_ptr<T>> get_components_by_type() const
        {
            static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

            std::vector<std::weak_ptr<T>> filtered_components;

            for (const auto &[id, component] : component_list)
            {
                auto casted = std::dynamic_pointer_cast<T>(component);
                if (casted)
                    filtered_components.push_back(casted);
            }

            return filtered_components;
        }

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

#include "engine/component/component_manager.inl"