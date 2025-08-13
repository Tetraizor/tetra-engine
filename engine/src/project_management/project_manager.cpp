#include "engine/project_management/project_manager.h"

#include "engine/serialization/json/json_value.h"
#include "engine/serialization/json/json_document.h"
#include "engine/serialization/json/json_serialization_context.h"
#include "engine/utils/io.h"
#include "engine/debug/logging/logger.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace Engine
{
    void ProjectManager::init(const std::string &project_file_path)
    {
        Logger::log_info("[ProjectManager] Loading project...");
        Logger::log_info("[ProjectManager] Path: " + project_file_path);

        project_path_abs = fs::path(project_file_path).parent_path();

        // Parse project file
        auto contents_opt = Utils::IO::read_file_contents(project_file_path);
        if (!contents_opt.has_value())
            throw std::runtime_error("Project file not found!");

        std::string project_file_contents = contents_opt.value();

        project_settings.deserialize(project_file_contents);

        // TODO: Check engine version and def version here.

        // TODO: Try to load latest open stage from project settings
    }
}
