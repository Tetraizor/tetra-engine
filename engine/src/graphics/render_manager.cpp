#include "render_manager.h"

#include "stage/stage_manager.h"
#include "component/component_manager.h"

#include "component/transform_component.h"

namespace Engine::Graphics
{
    RenderManager::RenderManager()
    {
        std::cout << "*** Spawn RenderManager ***" << std::endl;
    }

    void RenderManager::init()
    {
        stage_manager_ptr = &Engine::StageManager::get_instance();
        std::cout << "StageManager instance acquired.\n";

        Stage *stage = stage_manager_ptr->get_current_stage();
        if (!stage)
        {
            std::cerr << "get_current_stage() returned null!\n";
            return;
        }
        std::cout << "Current stage acquired.\n";

        ComponentManager *comp_mgr = &stage->get_component_manager();
        if (!comp_mgr)
        {
            std::cerr << "get_component_manager() returned null!\n";
            return;
        }
        std::cout << "ComponentManager acquired.\n";

        comp_mgr->component_created.subscribe(this, &RenderManager::on_component_added);
        comp_mgr->component_destroyed.subscribe(this, &RenderManager::on_component_destroyed);
        std::cout << "Event subscriptions done.\n";

        Entity *test = stage->get_entity_manager().create_entity("Test");
        test->add_component<TransformComponent>();
    }

    void RenderManager::on_component_added()
    {
        std::cout << "New component added!" << std::endl;
    }

    void RenderManager::on_component_destroyed()
    {
        std::cout << "Component removed!" << std::endl;
    }

    void RenderManager::render()
    {
        Stage *current_stage = stage_manager_ptr->get_current_stage();
        if (!current_stage)
            return;
    }
}