#include "component_registry.h"
#include "transform_component.h"

namespace Engine
{
    void TransformComponent::setup()
    {
        std::cout << "Position for " << entity->get_name()
                  << " is (" << x << ", " << y << ")" << std::endl;
    }
}

REGISTER_COMPONENT(TransformComponent, Engine::TransformComponent)
