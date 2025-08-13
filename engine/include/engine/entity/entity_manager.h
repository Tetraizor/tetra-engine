#pragma once

#include "engine/entity/entity_id.h"
#include "engine/serialization/serializable.h"
#include "engine/base/singleton.h"

#include <iostream>
#include <unordered_map>
#include <memory>

namespace Engine
{
    class Entity;
    class Stage;

    class EntityManager : public Serialization::Serializable, public Singleton<EntityManager>
    {
    public:
        EntityManager(Stage *owner_stage_ptr);

        Entity *create_entity(std::string name);

        void destroy_entity(const EntityID &id);
        void destroy_entity(const Entity &entity);

        /**
         * @brief Checks if entity with id exists in given managers entity list.
         *
         * @param id The unique identifier of the entity to look up.
         * @return whether entity is in the list or not.
         */
        bool has_entity(const EntityID &id) const;
        bool has_entity(const Entity &entity) const;

        /**
         * @brief Retrieves a pointer to the entity associated with the given EntityID.
         *
         * Searches the internal entity map for the provided ID. If an entity with that ID exists,
         * a raw pointer to the entity is returned. If not found, returns nullptr.
         *
         * @param id The unique identifier of the entity to look up.
         * @return Pointer to the Entity if found, or nullptr if no entity with the given ID exists.
         */
        Entity *get_entity_by_id(const EntityID &id) const;

        std::unordered_map<EntityID, std::unique_ptr<Entity>> *get_entity_list() { return &entity_list; }

        void setup();
        void update(const float delta_time);

        // Serialization
        void serialize(Serialization::SerializationContext &ctx) const override;
        void deserialize(Serialization::SerializationContext &ctx) override;

    private:
        std::unordered_map<EntityID, std::unique_ptr<Entity>> entity_list;

        std::vector<uint32_t> generations;
        std::vector<uint32_t> free_indices;

        Stage *stage = nullptr;
    };
}