#pragma once

#include "component.h"
#include "entity/entity.h"

namespace Engine
{
    class TransformComponent : public Component
    {
    public:
        float x = 0.0f, y = 0.0f;

        void serialize(Serialization::SerializationContext &ctx) const override
        {
            Component::serialize(ctx);

            ctx.write("x", x);
            ctx.write("y", y);
        }

        void deserialize(Serialization::SerializationContext &ctx) override
        {
            Component::deserialize(ctx);

            x = ctx.read<float>("x");
            y = ctx.read<float>("y");
        }

        void update(float delta_time) override {}
        void setup() override;
    };
}
