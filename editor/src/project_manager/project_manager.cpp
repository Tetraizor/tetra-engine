#include "project_manager/project_manager.h"
#include "serialization/json/json_value.h"
#include "serialization/json/json_document.h"
#include "serialization/json/json_serialization_context.h"
#include "utils/io.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace Editor
{
    ProjectManager::ProjectManager(const std::string &project_file_path)
    {
        std::cout << "[ProjectManager] Starting to load project..." << std::endl;
        std::cout << "[ProjectManager] Path: " << project_file_path << std::endl;

        project_file = project_file_path;
        project_path = fs::path(project_file_path).parent_path();

        auto contents_opt = Engine::Utils::IO::read_file_contents(project_file);
        if (!contents_opt.has_value())
            throw std::runtime_error("Project file not found!");

        std::string project_file_contents = contents_opt.value();

        Engine::Serialization::Json::JsonDocument doc(project_file_contents);

        auto root = doc.get_root();

        if (
            !root.has("project_name") ||
            !root.has("engine_version") ||
            !root.has("project_definition_version"))
        {
            throw std::runtime_error("Missing required field(s) in project file!");
        }

        project_name = root.get("project_name").as<std::string>().value();

        std::string engine_version = root.get("engine_version").as<std::string>().value();
        std::string project_definition_version = root.get("project_definition_version").as<std::string>().value();
        std::string last_open_stage = root.get("last_open_stage").as<std::string>().value();

        auto absolute_stage_path = get_project_path() / last_open_stage;

        // TODO: Check engine version and def version here.

        load_stage(absolute_stage_path.string());
    }

    ProjectManager::~ProjectManager() {}

    void ProjectManager::load_stage(const std::string &scene_path)
    {
        auto scene_contents_opt = Engine::Utils::IO::read_file_contents(scene_path);
        if (!scene_contents_opt.has_value())
        {
            std::cerr << "[ProjectManager] Failed to open scene: " << scene_path << std::endl;
            std::cerr << "[ProjectManager] Defaulting to empty scene..." << std::endl;

            // TODO: Open empty scene
            throw std::runtime_error("Opening new scene not implemented yet!");
        }

        std::string contents = scene_contents_opt.value();

        Engine::Serialization::Json::JsonDocument doc(contents);

        auto root = doc.get_root();

        std::unique_ptr<Engine::Stage> stage_ptr = std::make_unique<Engine::Stage>();
        Engine::JSONSerializationContext ctx(stage_ptr.get(), std::move(root));

        stage_ptr->deserialize(ctx);

        loaded_stages.emplace(stage_ptr->get_guid(), std::move(stage_ptr));
    }

    void ProjectManager::unload_stage(Engine::GUID &scene_guid)
    {
    }
}