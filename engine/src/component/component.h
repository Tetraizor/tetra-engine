#pragma once

#include "component/component_id.h"
#include "serialization/serializable.h"

namespace Engine
{
    class ComponentRegistry;

    class Component : public Serializable
    {
        friend class Entity;
        friend class ComponentManager;

    public:
        virtual ~Component() = default;

        virtual void update(float delta_time) = 0;
        virtual void setup() = 0;

        ComponentID get_id() { return id; }

    protected:
        ComponentID id;
        Entity *entity = nullptr;

    private:
        void set_entity(Entity *entity) { this->entity = entity; }
    };
}

#define REGISTER_COMPONENT_AUTO(ComponentName, ComponentClassName) \
    namespace                                                      \
    {                                                              \
        const bool registrar_##ComponentName = []() {                                               \
            Engine::ComponentRegistry::instance().register_type<ComponentClassName>(                \
                #ComponentName                \
            );                                                                                      \
            return true; }();         \
    }
