#pragma once

#include "stage/stage_manager.h"
#include "base/singleton.h"

namespace Engine::Graphics
{
    class RenderManager : public Singleton<RenderManager>
    {
    public:
        RenderManager();

        void init();
        void render();

        void on_component_added();
        void on_component_destroyed();

    private:
        StageManager *stage_manager_ptr = nullptr;
    };
}