#include "stage.h"
#include "viewport/viewport.h"
#include "entity/entity.h"
#include "component/component.h"

#include <memory>

namespace Engine
{
    Stage::Stage(bool headless) : headless(headless), entity_manager(this), component_manager(this) {}

    Stage::~Stage() = default;

    void Stage::setup()
    {
        entity_manager.setup();
    }

    void Stage::render()
    {
        if (is_headless())
            return;

        viewport->beginFrame();

        // TODO: Render things

        viewport->endFrame();
    }

    void Stage::update(float delta_time)
    {
        entity_manager.update(delta_time);
    }

    void Stage::attach_viewport(std::shared_ptr<Viewport> vp)
    {
        viewport = std::move(vp);
        headless = false;
    }

    void Stage::detach_viewport()
    {
        viewport.reset();
        headless = true;
    }

    bool Stage::is_headless() const
    {
        return headless || !viewport;
    }

    EntityManager &Stage::get_entity_manager()
    {
        return entity_manager;
    }

    ComponentManager &Stage::get_component_manager()
    {
        return component_manager;
    }
}
