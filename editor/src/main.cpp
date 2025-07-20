#include <iostream>
#include <memory>

#include "core.h"
#include "stage/stage.h"
#include "viewport/viewport.h"
#include "data/guid.h"

#include "component/transform_component.h"
#include "component/component_registry.h"

#include "project_manager/project_manager.h"

int main(int argc, char **argv)
{
    std::cout << "[Engine] Starting initialization... " << std::endl;

    std::cout << "[Engine] Running with " << argc << " arg(s). Arg(s):" << std::endl;

    try
    {
        std::vector<std::string> args;
        for (int i = 0; i < argc; i++)
        {
            args.push_back(argv[i]);
            std::cout << "[Engine] " << i << ": " << argv[i] << std::endl;
        }

        if (argc == 1)
        {
            // TODO: Initialize Project Manager
            throw std::runtime_error("[Engine] Project Manager is not implemented yet!");

            return -1;
        }
        else if (argc == 2)
        {
            // TODO: Initialize project directly from console
            std::cout << "[Engine] Launching project at path \"" << args.at(1) << "\""
                      << std::endl;

            try
            {
                Editor::ProjectManager project_manager(args.at(1));
            }
            catch (const std::exception &e)
            {
                std::cerr << "[Engine] Failed to launch project: " << e.what() << std::endl;
                return 1;
            }

            return -1;
        }
        else
        {
            std::cout << "[Engine] Program launched with invalid arguments, exiting..." << std::endl;
            return -1;
        }

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
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }

    return -1;
}