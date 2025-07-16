#include <iostream>
#include <memory>
#include "core.h"
#include "stage/stage.h"
#include "viewport/viewport.h"
#include "utils/guid.h"

#include "component/transform_component.h"
#include "component/component_registry.h"

int main()
{
    Engine::Core core;

    if (!core.initialize())
    {
        std::cerr << "Failed to initialize Core!" << std::endl;
        return -1;
    }

    auto stage = core.createStage(false);
    auto viewport = std::make_shared<Engine::Viewport>("Engine Window", 1280, 720);

    std::unique_ptr<Engine::TransformComponent> transform = Engine::ComponentRegistry::instance().create_typed<Engine::TransformComponent>();
    std::cout << "X: " << transform->x << std::endl;

    if (!viewport->initialize())
    {
        std::cerr << "Failed to initialize viewport!" << std::endl;
    }

    stage->attach_viewport(viewport);

    core.run();
    core.shutdown();

    return 0;
}