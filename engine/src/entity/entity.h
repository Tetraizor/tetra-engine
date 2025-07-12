#pragma once

#include "entity/entity_id.h"

#include <string>
#include <memory>
#include <typeindex>
#include <vector>

namespace Engine
{
    class Component;
    class EntityManager;

    class Entity
    {
        friend class EntityManager;

    public:
        explicit Entity(std::string name);
        ~Entity() = default;

        // Accessors
        EntityID get_id() const;
        Entity *get_parent() const;
        std::string get_name() const;
        std::vector<Entity *> get_children() const;

        // Components
        template <typename T>
        T *add_component() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            auto component = std::make_unique<T>();
            component->entity = this;

            T *ptr = component.get();

            components.push_back(std::move(component));

            return ptr;
        }

        template <typename T>
        T *get_component() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            for (const auto &comp : components)
            {
                if (auto ptr = dynamic_cast<T *>(comp.get()))
                    return ptr;
            }

            return nullptr;
        }

        template <typename T>
        bool has_component() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            return get_component<T>() != nullptr;
        }

        template <typename T>
        std::vector<T *> get_all_components_of_type() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            std::vector<T *> found_components;

            for (const auto &comp : components)
            {
                if (auto ptr = dynamic_cast<T *>(comp.get()))
                    found_components.push_back(ptr);
            }

            return found_components;
        }

        // Related to other entities
        void set_parent(const EntityID &parent);

        // Core
        void update(float deltaTime);
        void setup();

    private:
        // Relation to other entities
        EntityManager *entity_manager;
        EntityID parent_id;
        std::vector<EntityID> children_ids;

        // Self properties
        EntityID id;
        std::string name;
        std::vector<std::unique_ptr<Component>> components;

        void set_manager(EntityManager *entity_manager) { this->entity_manager = entity_manager; }
    };
}