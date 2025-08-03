#include "entity/entity.h"
#include "entity/entity_manager.h"
#include "component/component.h"

#include <cassert>

namespace Engine
{
    Entity::Entity(std::string name) : name(std::move(name)), components() {}

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
    void Entity::serialize(Serialization::SerializationContext &ctx) const
    {
        ctx.begin_object_key("id");
        id.serialize(ctx);
        ctx.end_object();

        ctx.write("name", name);
    }

    void Entity::deserialize(Serialization::SerializationContext &ctx)
    {
        ctx.begin_object_key("id");
        id.deserialize(ctx);
        ctx.end_object();

        name = ctx.read<std::string>("name");
    }
}