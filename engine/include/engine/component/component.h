#pragma once

#include "engine/component/component_id.h"
#include "engine/serialization/serializable.h"
#include "engine/base/engine_object.h"

namespace Engine
{
    class ComponentRegistry;
    class Entity;

    class Component : public Serialization::Serializable, public EngineObject
    {
        friend class Entity;
        friend class ComponentManager;

    public:
        virtual ~Component() = default;

        virtual void update(float delta_time) {};
        virtual void setup() {};

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

        const Entity *get_entity() const { return entity; }

    protected:
        ComponentID id;
        const Entity *entity = nullptr;

    private:
        void set_entity(const Entity *entity) { this->entity = entity; }
    };
}