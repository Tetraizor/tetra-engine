#pragma once

#include "component/component_id.h"
#include "serialization/serializable.h"

namespace Engine
{
    class ComponentRegistry;
    class Entity;

    class Component : public Serialization::Serializable
    {
        friend class Entity;
        friend class ComponentManager;

    public:
        virtual ~Component() = default;

        virtual void update(float delta_time) = 0;
        virtual void setup() = 0;

        ComponentID get_id() { return id; }

        void serialize(Serialization::SerializationContext &ctx) const override
        {
            ctx.begin_object_key("component_id");
            id.serialize(ctx);
            ctx.end_object();

            ctx.begin_object_key("owner_id");
            id.serialize(ctx);
            ctx.end_object();
        }

        void deserialize(Serialization::SerializationContext &ctx) override
        {
            ctx.begin_object_key("component_id");
            id.deserialize(ctx);
            ctx.end_object();

            ctx.begin_object_key("owner_id");
            id.deserialize(ctx);
            ctx.end_object();
        }

    protected:
        ComponentID id;
        Entity *entity = nullptr;

    private:
        void set_entity(Entity *entity) { this->entity = entity; }
    };
}