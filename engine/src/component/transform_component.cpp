

#include "transform_component.h"
#include "component_registry.h"
#include "utils/component_utils.h"

REGISTER_COMPONENT_TYPE(TransformComponent, Engine::TransformComponent, "Transform");

namespace Engine
{
    void TransformComponent::setup()
    {
        std::cout << "Position for " << entity->get_name() << " is (" << x << ", " << y << ")" << std::endl;
    }
}
