#include "editor.h"

namespace fs = std::filesystem;
using namespace Engine::Platform;

namespace Editor
{
    Editor::Editor(fs::path project_path)
        : project_manager(project_path.string()),
          editor_window(Window::Config{"Editor", 1280, 800, true, false})
    {
    }

    void Editor::initialize() {}
}