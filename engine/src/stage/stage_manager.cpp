#include "stage_manager.h"
#include "entity/entity.h"
#include "component/transform_component.h"

namespace Engine
{
    std::unique_ptr<Stage> StageManager::create_empty_stage()
    {
        auto stage_ptr = std::make_unique<Stage>();

        Entity *entity = stage_ptr->get_entity_manager().create_entity("New Object");
        entity->add_component<TransformComponent>();

        return stage_ptr;
    }

    void StageManager::load_new_stage()
    {
        current_stage = create_empty_stage();
    }
}