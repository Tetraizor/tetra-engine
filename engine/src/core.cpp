#include "core.h"

#include <SDL3/SDL.h>
#include <iostream>
#include <algorithm>

namespace Engine
{
    Core::Core() = default;

    Core::~Core()
    {
        shutdown();
    }

    bool Core::initialize()
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
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
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
            }

            float deltaTime = 0.016f;

            for (auto &stage : stages)
            {
                stage->update(deltaTime);
            }

            for (auto &stage : stages)
            {
                stage->render();
            }

            SDL_Delay(16);
        }
    }

    void Core::shutdown()
    {
        if (!running)
            return;

        stages.clear();

        SDL_Quit();
        running = false;
    }

    std::shared_ptr<Stage> Core::createStage(bool headless)
    {
        auto stage = std::make_shared<Stage>(headless);
        stages.push_back(stage);
        return stage;
    }

    void Core::destroyStage(std::shared_ptr<Stage> stage)
    {
        auto it = std::find(stages.begin(), stages.end(), stage);
        if (it != stages.end())
        {
            stages.erase(it);
        }
    }
}