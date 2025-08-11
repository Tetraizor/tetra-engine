#pragma once

#include "engine/entity/entity_id.h"
#include "engine/serialization/serialization_context.h"
#include "engine/serialization/serializable.h"

#include "engine/stage/stage_manager.h"
#include "engine/component/component_manager.h"

#include <string>
#include <memory>
#include <typeindex>
#include <vector>

namespace Engine
{
    class Component;
    class EntityManager;

    class Entity : public Serialization::Serializable
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
        T *add_component()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

            try
            {
                auto &stage_manager = StageManager::get_instance();
                Stage *stage = stage_manager.get_current_stage();
                if (stage == nullptr)
                    throw std::runtime_error("Current scene is null");

                ComponentManager &component_manager = stage->get_component_manager();

                std::shared_ptr<Component> component_ptr = component_manager.create_component<T>(id);
                Component *ptr = component_ptr.get();

                components.push_back(component_ptr);
                return static_cast<T *>(ptr);
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error(std::string("Failed to add component: ") + e.what());
            }
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

        // Serialization
        void serialize(Serialization::SerializationContext &ctx) const override;
        void deserialize(Serialization::SerializationContext &ctx) override;

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
        std::vector<std::shared_ptr<Component>> components;

        void set_manager(EntityManager *entity_manager) { this->entity_manager = entity_manager; }
    };
}