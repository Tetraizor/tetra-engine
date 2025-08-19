#pragma once

#include "engine/component/component_registry.h"
#include "engine/graphics/viewport.h"
#include "engine/math/vector3.h"
#include "engine/math/matrix4.h"

namespace Engine::Graphics
{
    class Viewport;
}

namespace Engine
{
    using namespace Math;

    class Camera3D : public Component
    {

    public:
        void update(float delta_time) override;
        void setup() override;

        virtual void render();
        std::function<void(const Matrix4 &, const Matrix4 &)> on_render_scene;

        Graphics::Viewport *get_viewport() { return viewport.get(); }

        float get_fov_degrees() { return fov_degrees; }
        float get_near_plane() { return near_plane; }
        float get_far_plane() { return far_plane; }

        void deserialize(Serialization::SerializationContext &ctx) override { Component::serialize(ctx); }
        void serialize(Serialization::SerializationContext &ctx) const override { Component::serialize(ctx); }

    private:
        float fov_degrees = 60.0f;
        float near_plane = 0.1f;
        float far_plane = 1000.0f;

        std::unique_ptr<Graphics::Viewport> viewport;

        void ensure_viewport(int width, int height);
    };
}

REGISTER_COMPONENT(Camera3D, Engine::Camera3D)