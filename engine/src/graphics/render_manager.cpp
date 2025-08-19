#include "engine/graphics/render_manager.h"

#include "engine/stage/stage_manager.h"
#include "engine/component/component_manager.h"

#include "engine/component/3d/transform_3d.h"

namespace Engine::Graphics
{
    RenderManager::RenderManager() : stage_manager(StageManager::get_instance()) {}
    RenderManager::~RenderManager()
    {
        ComponentManager &component_manager = stage_manager.get_current_stage()->get_component_manager();

        component_manager.component_created.unsubscribe(on_component_created_token);
        component_manager.component_destroyed.unsubscribe(on_component_destroyed_token);
    }

    void RenderManager::init()
    {
        Stage *stage_ptr = stage_manager.get_current_stage();
        if (stage_ptr == nullptr)
            throw std::runtime_error("current_stage is null");

        ComponentManager &component_manager = stage_ptr->get_component_manager();

        camera_components = component_manager.get_components_by_type<Camera3D>();

        on_component_created_token = component_manager.component_created.subscribe(this, &RenderManager::on_component_created);
        on_component_destroyed_token = component_manager.component_destroyed.subscribe(this, &RenderManager::on_component_destroyed);

        Entity *test = stage_ptr->get_entity_manager().create_entity("Test");
        test->add_component<Transform3D>();
    }

    void RenderManager::on_component_created(std::weak_ptr<Component> component_ptr)
    {
        auto component_shared_ptr = component_ptr.lock();
        assert(component_shared_ptr);

        std::shared_ptr<Camera3D> camera = std::dynamic_pointer_cast<Camera3D>(component_shared_ptr);
        if (camera)
            camera_components.push_back(camera);
    }

    void RenderManager::on_component_destroyed(std::weak_ptr<Component> component_ptr)
    {
        ComponentManager &component_manager = stage_manager.get_current_stage()->get_component_manager();
        camera_components = component_manager.get_components_by_type<Camera3D>();
    }

    void RenderManager::render()
    {
        for (auto &camera_component_weak : camera_components)
        {
            assert(!camera_component_weak.expired());

            std::shared_ptr<Camera3D> camera_component_ptr = camera_component_weak.lock();
            camera_component_ptr->render();
        }
    }

    void RenderManager::present_to_window(Platform::Window *window)
    {
        if (!window)
            return;

        if (camera_components.empty())
            return;

        std::shared_ptr<Camera3D> primary;
        for (auto &wp : camera_components)
        {
            primary = wp.lock();
            if (primary)
                break;
        }

        if (!primary)
            return;

        auto vp = primary->get_viewport();
        if (!vp || !vp->is_valid())
            return;

        glBindFramebuffer(GL_READ_FRAMEBUFFER, vp->get_fbo());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        int srcW = vp->get_width();
        int srcH = vp->get_height();
        int dstW = window->get_width();
        int dstH = window->get_height();

        glBlitFramebuffer(
            0, 0, srcW, srcH,
            0, 0, dstW, dstH,
            GL_COLOR_BUFFER_BIT, GL_LINEAR);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        window->swap_buffers();
    }
}
