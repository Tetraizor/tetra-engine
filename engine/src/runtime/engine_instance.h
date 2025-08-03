#pragma once

#include <vector>
#include <string>

#include "base/singleton.h"
#include "project_management/project_manager.h"
#include "graphics/render_manager.h"
#include "stage/stage_manager.h"
#include "asset/asset_manager.h"

namespace Engine
{
    class EngineInstance : public Singleton<EngineInstance>
    {
    public:
        EngineInstance();
        ~EngineInstance() = default;

        int run(std::vector<std::string> arguments);

        float get_delta_time() { return delta_time; }
        float get_fps() { return 1.0f / delta_time; }

        /// @brief Setup engine instance managers before running the engine
        /// @param project_path Path containing the "project.tetra" file
        virtual void init(const std::string &project_path);

    protected:
        StageManager stage_manager;
        ProjectManager project_manager;
        Graphics::RenderManager render_manager;
        Asset::AssetManager asset_manager;

        float delta_time;
    };
}