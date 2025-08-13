#pragma once

#include <vector>
#include <string>

#include "engine/base/singleton.h"
#include "engine/project_management/project_manager.h"
#include "engine/graphics/render_manager.h"
#include "engine/stage/stage_manager.h"
#include "engine/asset/asset_manager.h"

namespace Engine
{
    class EngineInstance : public Singleton<EngineInstance>
    {
    public:
        EngineInstance();
        ~EngineInstance() = default;

        int run(std::vector<std::string> arguments);

        float get_delta_time() const { return delta_time; }
        float get_fps() const
        {

            std::stringstream stream;
            stream << std::fixed << std::setprecision(10) << delta_time;
            std::string s = stream.str();

            Logger::log_info(std::string("get_fps: ") + s);

            float result = 1.0f / delta_time;
            return result;
        }

        /// @brief Setup engine instance managers before running the engine
        /// @param project_path Path containing the "project.tetra" file
        virtual void init(const std::string &project_path);

    protected:
        float delta_time;
    };
}