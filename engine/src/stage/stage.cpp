#include "stage.h"
#include "viewport/viewport.h"
#include "entity/entity.h"
#include "component/component.h"
#include "component/component_manager.h"

#include <memory>

namespace Engine
{
    Stage::Stage(bool headless) : headless(headless)
    {
        entity_manager = std::make_unique<EntityManager>(this);
        component_manager = std::make_unique<ComponentManager>(this);
    }

    Stage::~Stage() = default;

    void Stage::setup()
    {
        entity_manager->setup();
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
        entity_manager->update(delta_time);
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
        return *entity_manager;
    }

    ComponentManager &Stage::get_component_manager()
    {
        return *component_manager;
    }

    // Serialization
    void Stage::serialize(SerializationContext &ctx) const
    {
        std::cout << "[Stage] Starting to serialize..." << std::endl;

        entity_manager->serialize(ctx);
        component_manager->serialize(ctx);

        std::cout << "[Stage] Serializing finished." << std::endl;
    }

    void Stage::deserialize(SerializationContext &ctx)
    {
        std::cout << "[Stage] Starting to deserialize..." << std::endl;

        std::cout << "[Stage] Stage name: " << ctx.read_string("stage_name") << std::endl;
        guid = ctx.read_guid("guid");
        name = ctx.read_string("name");

        entity_manager->deserialize(ctx);
        component_manager->deserialize(ctx);

        std::cout << "[Stage] Deserializing finished for " << get_guid().to_string() << std::endl;
    }
}
