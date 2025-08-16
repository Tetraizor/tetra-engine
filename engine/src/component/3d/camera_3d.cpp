#include "engine/component/3d/camera_3d.h"

#include "engine/entity/entity.h"
#include "engine/component/3d/transform_3d.h"

namespace Engine
{
    void Camera3DComponent::setup()
    {
    }

    void Camera3DComponent::update(float delta_time)
    {
    }

    void Camera3DComponent::ensure_viewport(int width, int height)
    {
        if (!viewport)
        {
            viewport = std::make_unique<Graphics::Viewport>(width, height);

            if (!viewport->initialize())
                throw std::runtime_error("Failed to initialize camera viewport");
        }
        else
        {
            if (viewport->get_width() != width || viewport->get_height() != height)
                viewport->resize(width, height);
        }
    }

    void Camera3DComponent::render()
    {
        int desired_width = 1280;
        int desired_height = 720;

        if (viewport)
        {
            desired_width = viewport->get_width();
            desired_height = viewport->get_height();
        }

        ensure_viewport(desired_width, desired_height);

        if (!viewport || !viewport->is_valid())
            return;

        Vector3 position(0.0f, 0.0f, 5.0f);
        Vector3 forward(0.0f, 0.0f, -1.0f);
        Vector3 up(0.0f, 1.0f, 0.0f);

        if (get_entity())
        {
            auto transform = get_entity()->get_component<Transform3D>();
            if (transform)
            {
            }
        }
    }
}
