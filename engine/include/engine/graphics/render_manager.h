#pragma once

#include "engine/stage/stage_manager.h"
#include "engine/base/singleton.h"
#include "engine/platform/window.h"

#include "engine/component/3d/camera_3d.h"

namespace Engine::Graphics
{
    class RenderManager : public Singleton<RenderManager>
    {
    public:
        RenderManager();
        ~RenderManager();

        void init();
        void render();

        void on_component_created(std::weak_ptr<Component> component_ptr);
        void on_component_destroyed(std::weak_ptr<Component> component_ptr);
        void on_component_destroyed();

        void present_to_window(Engine::Platform::Window *window);

    private:
        size_t on_component_created_token;
        size_t on_component_destroyed_token;

        StageManager &stage_manager;
        std::vector<std::weak_ptr<Camera3DComponent>> camera_components;
    };
}