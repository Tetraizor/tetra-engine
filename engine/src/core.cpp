#include "core.h"
#include "platform/window_manager.h"

namespace Engine
{
    Core::Core() = default;

    Core::~Core()
    {
        shutdown();
    }

    bool Core::initialize()
    {
        windowManager = std::make_unique<WindowManager>();
        if (!windowManager->initialize())
        {
            return false;
        }

        mainWindowId = windowManager->createWindow("TetraEngine", 1280, 720);
        if (!mainWindowId)
        {
            return false;
        }

        running = true;
        return true;
    }

    void Core::run()
    {
        SDL_Event event;
        while (running)
        {
            while (windowManager->pollEvent(event))
            {
                if (event.type == SDL_EVENT_QUIT)
                    running = false;

                // Pass event to input module
            }

            update();
            render();
        }
    }

    void Core::shutdown()
    {
        if (windowManager)
        {
            windowManager->shutdown();
            windowManager = nullptr;
        }

        running = false;
    }

    void Core::update()
    {
    }

    void Core::render()
    {
        if (!mainWindowId.has_value())
            return;

        auto *mainWindow = windowManager->getWindow(*mainWindowId);
        if (!mainWindow || mainWindow->isHeadless())
            return;

        SDL_Renderer *renderer = mainWindow->getSDLRenderer();
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        // Draw scene elements

        SDL_RenderPresent(renderer);
    }
}