#include "editor/runtime/editor_instance.h"

namespace Editor
{
    EditorInstance::EditorInstance() : EngineInstance() {}

    void EditorInstance::init(const std::string &project_path)
    {
        EngineInstance::init(project_path);
    }
}