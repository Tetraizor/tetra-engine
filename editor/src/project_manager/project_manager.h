#include "stage/stage.h"
#include "data/guid.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

namespace Editor
{
    class ProjectManager
    {
    public:
        ProjectManager(const std::string &project_file_path);
        ~ProjectManager();

        bool check_project_file(const std::string &project_file_path) const;

        void load_stage(const std::string &scene_path);
        void unload_stage(Engine::GUID &scene_guid);

        std::string project_file;

        fs::path get_project_path() { return project_path; }

    private:
        std::string project_name;
        fs::path project_path;

        std::unordered_map<Engine::GUID, std::unique_ptr<Engine::Stage>> loaded_stages;
    };
}