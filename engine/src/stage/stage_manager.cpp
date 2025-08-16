#include "engine/stage/stage_manager.h"

namespace Engine
{
    std::unique_ptr<Stage> StageManager::create_empty_stage()
    {
        auto stage_ptr = std::make_unique<Stage>();

        return stage_ptr;
    }

    void StageManager::load_new_stage()
    {
        current_stage = create_empty_stage();
    }
}
