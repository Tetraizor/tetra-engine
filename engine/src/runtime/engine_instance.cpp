#include "engine_instance.h"

#include <iostream>
#include <chrono>
#include <filesystem>

#include "engine.h"
#include "stage/stage.h"
#include "debug/logging/console_sink.h"

namespace Engine
{
    EngineInstance::EngineInstance() {}

    int EngineInstance::run(std::vector<std::string> arguments)
    {
        Logger::log_info("[EngineInstance] Starting up...");
        Logger::log_info("[EngineInstance] Start arguments: ");

        for (std::string arg : arguments)
        {
            Logger::log_info('\t' + arg);
        }

        StageManager &stage_manager = StageManager::get_instance();
        Stage *current_stage_ptr = stage_manager.get_current_stage();
        if (current_stage_ptr == nullptr)
            throw std::runtime_error("Current stage is null");

        // Start main engine loop
        using clock = std::chrono::high_resolution_clock;

        constexpr double fixed_timestep = 1.0 / 60.0; // Physics tick rate
        double accumulator = 0.0;

        auto previous_time = clock::now();

        while (!stage_manager.get_current_stage()->has_requested_shutdown())
        {
            auto current_time = clock::now();
            std::chrono::duration<double> delta_time_duration = current_time - previous_time;
            previous_time = current_time;

            double frame_time = delta_time_duration.count();
            accumulator += frame_time;

            // TODO: Do input handling with something like this?
            // current_stage->handle_input();

            // Wait idly for physics simulation if frame time is too low
            while (accumulator >= fixed_timestep)
            {
                // TODO: Update physics backend
                stage_manager.get_current_stage()->physics_update(fixed_timestep);
                accumulator -= fixed_timestep;
            }

            delta_time = frame_time;

            stage_manager.get_current_stage()->update(frame_time);
            Graphics::RenderManager::get_instance().render();

            // TODO: Add vsync / framerate cap / sleep?
        }

        return 0; // Handle shutdown gracefully
    }

    void EngineInstance::init(const std::string &project_path)
    {
        std::shared_ptr<LogSink> console_sink = std::make_shared<ConsoleSink>();
        Logger::get_instance().add_sink(console_sink);

        ProjectManager::get_instance().init(project_path + "/project.tetra");
        Asset::AssetManager::get_instance().init(project_path);
        StageManager::get_instance().load_new_stage();

        Graphics::RenderManager::get_instance().init();
    }
}