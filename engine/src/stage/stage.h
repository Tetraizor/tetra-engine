#pragma once

#include <memory>
#include "viewport/viewport.h"
#include "entity/entity_manager.h"
#include "serialization/serializable.h"
#include "data/guid.h"

namespace Engine::Serialization
{
    class SerializationContext;
}

namespace Engine
{
    using Engine::Serialization::SerializationContext;

    class Viewport;
    class ComponentManager;

    class Stage : public Engine::Serialization::Serializable
    {
    public:
        explicit Stage(bool headless = false);
        ~Stage();

        void setup();
        void render();
        void update(float deltaTime);

        void attach_viewport(std::shared_ptr<Viewport> viewport);
        void detach_viewport();

        GUID get_guid() const { return guid; }
        std::string get_name() const { return name; }

        bool is_headless() const;

        EntityManager &get_entity_manager();
        ComponentManager &get_component_manager();

        void serialize(SerializationContext &ctx) const override;
        void deserialize(SerializationContext &ctx) override;

    private:
        GUID guid;
        std::string name;

        bool headless;
        std::shared_ptr<Viewport> viewport;

        std::unique_ptr<EntityManager> entity_manager;
        std::unique_ptr<ComponentManager> component_manager;
    };
}