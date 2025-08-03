#pragma once

#include <memory>
#include "graphics/viewport.h"
#include "entity/entity_manager.h"
#include "serialization/serializable.h"
#include "data/guid.h"
#include "base/runtime_object_base.h"

namespace Engine::Serialization
{
    class SerializationContext;
}

namespace Engine
{
    using Engine::Serialization::SerializationContext;

    class ComponentManager;

    class Stage : public Serialization::Serializable, public RuntimeObjectBase
    {
    public:
        explicit Stage(bool headless = false);
        ~Stage();

        void setup();
        void start();
        void render();
        void update(float delta_time);
        void physics_update(float delta_time);

        GUID get_guid() const { return guid; }
        std::string get_name() const { return name; }

        bool is_headless() const;
        bool has_requested_shutdown() const;
        void request_shutdown() { requested_shutdown = true; };

        EntityManager &get_entity_manager();
        ComponentManager &get_component_manager();

        void serialize(SerializationContext &ctx) const override;
        void deserialize(SerializationContext &ctx) override;

    private:
        GUID guid;
        std::string name;

        bool headless;
        bool requested_shutdown = false;
        std::shared_ptr<Graphics::Viewport> viewport;

        std::unique_ptr<EntityManager> entity_manager;
        std::unique_ptr<ComponentManager> component_manager;
    };
}