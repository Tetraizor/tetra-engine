#include "engine/component/3d/camera_3d.h"

#include "engine/runtime/engine_instance.h"
#include "engine/entity/entity.h"
#include "engine/component/3d/transform_3d.h"

namespace Engine
{
    void Camera3D::setup()
    {
    }

    void Camera3D::update(float delta_time)
    {
    }

    void Camera3D::ensure_viewport(int width, int height)
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

    void Camera3D::render()
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
                position = transform->get_position();
                forward = transform->get_forward();
                up = transform->get_up();
            }
        }

        Matrix4 view = Matrix4::look_at(position, position + forward, up);
        float aspect = static_cast<float>(viewport->get_width() / viewport->get_height());
        Matrix4 proj = Matrix4::perspective(fov_degrees * DEG2RAD, aspect, near_plane, far_plane);

        viewport->begin_frame();

        float time_passed = EngineInstance::get_instance().get_time_passed();

        Logger::log_info(std::to_string(time_passed));

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (on_render_scene)
            on_render_scene(view, proj);

        viewport->end_frame();
    }
}
