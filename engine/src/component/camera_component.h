#pragma once

#include "component/component_registry.h"
#include "graphics/viewport.h"

namespace Engine
{
    class CameraComponent : public Component
    {
    public:
        void update(float delta_time) override;
        void setup() override;

        virtual void render();

        void serialize(Serialization::SerializationContext &ctx) const override
        {
            Component::serialize(ctx);
        }

        void deserialize(Serialization::SerializationContext &ctx) override
        {
            Component::serialize(ctx);
        }

    private:
        std::unique_ptr<Graphics::Viewport> viewport;
    };
}