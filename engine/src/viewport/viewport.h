#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace Engine
{
    class Viewport
    {
    public:
        Viewport(const std::string &title, int width, int height);
        ~Viewport();

        bool initialize();
        void shutdown();

        void beginFrame();
        void endFrame();

        bool isValid() const;

    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;

        std::string title;

        int width = 0;
        int height = 0;
    };
}