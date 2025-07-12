#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>

#include "entity/entity_id.h"

namespace Engine
{
    class Entity;

    class EntityManager
    {
    public:
        Entity *create_entity(std::string name);

        void destroy_entity(const EntityID &id);
        void destroy_entity(const Entity &entity);

        bool has_entity(const EntityID &id) const;
        bool has_entity(const Entity &entity) const;

        Entity *get_entity_by_id(const EntityID &id) const;

        void setup();
        void update(float delta_time);

    private:
        std::unordered_map<EntityID, std::unique_ptr<Entity>> entity_list;

        std::vector<uint32_t> generations;
        std::vector<uint32_t> free_indices;

        EntityID create_new_id();
    };
}