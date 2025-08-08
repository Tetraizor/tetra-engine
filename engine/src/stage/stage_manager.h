#pragma once

#include "stage/stage.h"
#include "data/guid.h"

#include "base/singleton.h"
#include "engine.h"

#include <memory>

namespace Engine
{
    class StageManager : public Singleton<StageManager>
    {
    public:
        StageManager()
        {
            Logger::log("[StageManager] Initialization complete", LogLevel::Info);
        }

        Stage *get_current_stage()
        {
            return current_stage.get();
        }

        std::unique_ptr<Stage> create_empty_stage();

        void load_new_stage();
        void load_stage();

    private:
        std::unique_ptr<Stage> current_stage = nullptr;
        std::unordered_map<GUID, std::unique_ptr<Stage>> stage_registry;
    };
}