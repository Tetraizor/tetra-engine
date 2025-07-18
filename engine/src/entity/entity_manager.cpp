#include "entity/entity_manager.h"
#include "entity/entity_id.h"
#include "entity/entity.h"
#include "component/component.h"

#include <cassert>

namespace Engine
{
    EntityManager::EntityManager(Stage *owner_stage_ptr)
    {
        stage = owner_stage_ptr;

        std::cout << "[EntityManager] Initialized successfully!" << std::endl;
    }

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

    // Serialization
    void EntityManager::serialize(SerializationContext &ctx) const
    {
        ctx.begin_array("entities");

        for (const auto &[id, entity_ptr] : entity_list)
        {
            entity_ptr->serialize(ctx);
        }

        ctx.end_array();
    }

    void EntityManager::deserialize(SerializationContext &ctx)
    {
        entity_list.clear();
        generations.clear();
        free_indices.clear();

        ctx.begin_array("entities");

        for (int i = 0; i < ctx.array_size(); i++)
        {
            ctx.begin_object();

            auto entity = std::make_unique<Entity>("");
            entity->deserialize(ctx);

            EntityID id = entity->get_id();

            if (id.index >= generations.size())
            {
                generations.resize(id.index + 1, 0);
            }
            generations[id.index] = id.generation;

            entity->set_manager(this);
            entity_list[id] = std::move(entity);

            ctx.end_object();
        }

        ctx.end_array();
    }
}