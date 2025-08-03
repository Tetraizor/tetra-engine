#include "component_registry.h"
#include "transform_component.h"

namespace Engine
{
    void TransformComponent::setup()
    {
        std::cout << "Position for " << entity->get_name()
                  << " is (" << x << ", " << y << ")" << std::endl;
    }

    void TransformComponent::update(float delta_time)
    {
        std::cout << "Transform is alive!" << std::endl;
    }
}

REGISTER_COMPONENT(TransformComponent, Engine::TransformComponent)
