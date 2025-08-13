#include "engine/stage/stage.h"

#include "engine/graphics/viewport.h"
#include "engine/entity/entity.h"
#include "engine/component/component.h"
#include "engine/component/component_manager.h"

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

    void Stage::start()
    {
    }

    void Stage::render()
    {
        if (is_headless())
            return;

        viewport->begin_frame();

        // TODO: Render things

        viewport->end_frame();
    }

    void Stage::update(const float delta_time)
    {
        entity_manager->update(delta_time);
    }

    void Stage::physics_update(float delta_time)
    {
    }

    bool Stage::is_headless() const
    {
        return headless || !viewport;
    }

    bool Stage::has_requested_shutdown() const
    {
        return requested_shutdown;
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
        entity_manager->serialize(ctx);
        component_manager->serialize(ctx);
    }

    void Stage::deserialize(SerializationContext &ctx)
    {
        std::string guid_raw = ctx.read<std::string>("guid");
        guid = GUID::from_string(guid_raw);

        name = ctx.read<std::string>("name");

        ctx.begin_object_key("entity_manager");
        entity_manager->deserialize(ctx);
        ctx.end_object();

        ctx.begin_object_key("component_manager");
        component_manager->deserialize(ctx);
        ctx.end_object();
    }
}
