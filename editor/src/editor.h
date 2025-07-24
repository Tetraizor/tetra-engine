#include "project_manager/project_manager.h"
#include "platform/window.h"

#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace Engine::Platform;

namespace Editor
{
    class Editor
    {
    public:
        Editor(fs::path project_path);
        ~Editor() = default;

        void initialize();

        ProjectManager *get_project_manager() { return &project_manager; }
        Window *get_editor_window() { return &editor_window; }

    private:
        ProjectManager project_manager;
        Window editor_window;
    };
}