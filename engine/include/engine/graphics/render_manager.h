#pragma once

#include "engine/stage/stage_manager.h"
#include "engine/base/singleton.h"

#include "engine/component/camera_component.h"

namespace Engine::Graphics
{
    class RenderManager : public Singleton<RenderManager>
    {
    public:
        RenderManager();

        void init();
        void render();

        void on_component_created(std::weak_ptr<Component> component_ptr);
        void on_component_destroyed(std::weak_ptr<Component> component_ptr);
        void on_component_destroyed();

    private:
        StageManager &stage_manager;
        std::vector<std::weak_ptr<CameraComponent>> camera_components;
    };
}