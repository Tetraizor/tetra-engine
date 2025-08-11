#pragma once

#include <vector>
#include <string>

#include "engine/runtime/engine_instance.h"

namespace Editor
{
    using namespace Engine;

    class EditorInstance : public EngineInstance
    {
    public:
        EditorInstance();

        void init(const std::string &project_path) override;

    private:
    };
}