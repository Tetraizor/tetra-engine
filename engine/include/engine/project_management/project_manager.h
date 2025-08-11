#include "engine/stage/stage.h"
#include "engine/data/guid.h"
#include "engine/project_management/project_settings.h"

#include "engine/base/singleton.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

namespace Engine
{
    class ProjectManager : public Singleton<ProjectManager>
    {
    public:
        ~ProjectManager() = default;

        ProjectSettings get_project_settings() { return project_settings; }
        fs::path get_project_path() { return project_path_abs; }

        void init(const std::string &project_file_path);

    private:
        fs::path project_path_abs;

        ProjectSettings project_settings;

        std::unordered_map<GUID, std::unique_ptr<Stage>> loaded_stages;
    };
}