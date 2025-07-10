#pragma once

#include <SDL3/SDL.h>

namespace Engine
{
    class Window
    {
    public:
        bool initialize();
        void shutdown();

        bool pollEvent(SDL_Event &event);

        SDL_Window *getSDLWindow();
        SDL_Renderer *getSDLRenderer();

    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
    };
}