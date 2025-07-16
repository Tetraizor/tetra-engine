#include "entity/entity.h"
#include "entity/entity_manager.h"
#include "component/component.h"

#include <cassert>

namespace Engine
{
    Entity::Entity(std::string name) : name(std::move(name)) {}

    std::string Entity::get_name() const
    {
        return name;
    }

    EntityID Entity::get_id() const
    {
        return id;
    }

    Entity *Entity::get_parent() const
    {
        assert(entity_manager && "Entity has no assigned EntityManager");

        return entity_manager->get_entity_by_id(parent_id);
    }

    std::vector<Entity *> Entity::get_children() const
    {
        assert(entity_manager && "Entity has no assigned EntityManager");

        int children_count = children_ids.size();
        std::vector<Entity *> children;
        children.reserve(children_count);

        for (const EntityID &id : children_ids)
        {
            Entity *entity_ptr = entity_manager->get_entity_by_id(id);
            assert(entity_ptr != nullptr && "Children ID is from a destroyed object.");
            children.push_back(entity_ptr);
        }

        return children;
    }

    void Entity::set_parent(const EntityID &parent_id)
    {
        this->parent_id = parent_id;
    }

    void Entity::update(float deltaTime)
    {
        for (auto &component : components)
        {
            assert(component && "Component reference is not valid.");
            component->update(deltaTime);
        }
    }

    void Entity::setup()
    {
        for (auto &component : components)
        {
            assert(component && "Component reference is not valid.");
            component->setup();
        }
    }

    // Serialization
    void Entity::serialize(SerializationContext &ctx) const
    {
        ctx.begin_object();

        ctx.write_UInt(id.index, "id_index");
        ctx.write_UInt(id.generation, "id_generation");
        ctx.write_string(name, "name");

        for (size_t i = 0; i < components.size(); i++)
        {
            components[i]->serialize(ctx);
        }

        ctx.end_object();
    }

    void Entity::deserialize(SerializationContext &ctx)
    {
        uint32_t id_index = ctx.read_UInt("id_index");
        uint32_t id_generation = ctx.read_UInt("id_generation");

        id = {id_index, id_generation};
        name = ctx.read_string("name");
    }
}