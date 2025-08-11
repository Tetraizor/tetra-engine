#include "engine/graphics/render_manager.h"

#include "engine/engine.h"

#include "engine/stage/stage_manager.h"
#include "engine/component/component_manager.h"

#include "engine/component/transform_component.h"

namespace Engine::Graphics
{
    RenderManager::RenderManager() : stage_manager(StageManager::get_instance()) {}

    void RenderManager::init()
    {
        Stage *stage_ptr = stage_manager.get_current_stage();
        if (stage_ptr == nullptr)
            throw std::runtime_error("current_stage is null");

        ComponentManager &component_manager = stage_ptr->get_component_manager();

        camera_components = component_manager.get_components_by_type<CameraComponent>();

        component_manager.component_created.subscribe(this, &RenderManager::on_component_created);
        component_manager.component_destroyed.subscribe(this, &RenderManager::on_component_destroyed);

        Entity *test = stage_ptr->get_entity_manager().create_entity("Test");
        test->add_component<TransformComponent>();
    }

    void RenderManager::on_component_created(std::weak_ptr<Component> component_ptr)
    {
        auto component_shared_ptr = component_ptr.lock();
        assert(component_shared_ptr);

        std::shared_ptr<CameraComponent> camera = std::dynamic_pointer_cast<CameraComponent>(component_shared_ptr);
        if (camera)
            camera_components.push_back(camera);
    }

    void RenderManager::on_component_destroyed(std::weak_ptr<Component> component_ptr)
    {
        ComponentManager &component_manager = stage_manager.get_current_stage()->get_component_manager();
        camera_components = component_manager.get_components_by_type<CameraComponent>();
    }

    void RenderManager::render()
    {
        for (auto &camera_component_weak : camera_components)
        {
            assert(!camera_component_weak.expired());
            std::shared_ptr<CameraComponent> camera_component_ptr = camera_component_weak.lock();

            camera_component_ptr->render();
        }
    }
}
