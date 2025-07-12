#include "entity/entity_manager.h"
#include "entity/entity_id.h"
#include "entity/entity.h"
#include "component/component.h"

#include <cassert>

namespace Engine
{
    Entity *EntityManager::create_entity(std::string name)
    {
        assert(generations.size() == entity_list.size() && "entity_list size and generations size do not match.");

        uint32_t index;

        if (!free_indices.empty())
        {
            index = free_indices.back();
            free_indices.pop_back();
        }
        else
        {
            index = static_cast<uint32_t>(generations.size());
            generations.push_back(0);
        }

        EntityID id = {index, generations[index]};

        auto entity = std::make_unique<Entity>(name);
        entity->set_manager(this);
        entity->id = id;

        entity_list.emplace(id, std::move(entity));

        return entity.get();
    }

    void EntityManager::destroy_entity(const EntityID &id)
    {
        auto it = entity_list.find(id);

        if (it != entity_list.end())
        {
            entity_list.erase(it);

            generations[id.index]++;
            free_indices.push_back(id.index);
        }
    }

    void EntityManager::destroy_entity(const Entity &entity)
    {
        destroy_entity(entity.get_id());
    }

    bool EntityManager::has_entity(const EntityID &id) const
    {
        auto it = entity_list.find(id);
        return it != entity_list.end();
    }

    bool EntityManager::has_entity(const Entity &entity) const
    {
        return has_entity(entity.get_id());
    }

    /**
     * @brief Retrieves a pointer to the entity associated with the given EntityID.
     *
     * Searches the internal entity map for the provided ID. If an entity with that ID exists,
     * a raw pointer to the entity is returned. If not found, returns nullptr.
     *
     * @param id The unique identifier of the entity to look up.
     * @return Pointer to the Entity if found, or nullptr if no entity with the given ID exists.
     */
    Entity *EntityManager::get_entity_by_id(const EntityID &id) const
    {
        auto found_entity = entity_list.find(id);
        if (found_entity != entity_list.end())
            return found_entity->second.get();

        return nullptr;
    }

    void EntityManager::setup()
    {
        for (auto &entity_pair : entity_list)
        {
            entity_pair.second.get()->setup();
        }
    }

    void EntityManager::update(float delta_time)
    {
        for (auto &entity_pair : entity_list)
        {
            entity_pair.second.get()->update(delta_time);
        }
    }
}