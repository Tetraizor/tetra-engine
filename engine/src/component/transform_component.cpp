#include "component.h"
#include "transform_component.h"
#include "utils/component_utils.h"

namespace Engine
{
    void TransformComponent::setup()
    {
        std::cout << "Position for " << entity->get_name()
                  << " is (" << x << ", " << y << ")" << std::endl;
    }

    void forceLinkTransformComponentRegistration() {}
}

REGISTER_COMPONENT_AUTO(TransformComponent, Engine::TransformComponent)
