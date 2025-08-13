#pragma once

#include "engine/entity/entity_id.h"
#include "engine/serialization/serialization_context.h"
#include "engine/serialization/serializable.h"

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
        T *add_component();

        template <typename T>
        T *get_component() const;

        template <typename T>
        bool has_component() const;

        template <typename T>
        std::vector<T *> get_all_components_of_type() const;

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

#include "entity.inl"