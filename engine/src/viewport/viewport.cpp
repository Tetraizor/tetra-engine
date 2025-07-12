#include "viewport.h"
#include <iostream>

namespace Engine
{
    Viewport::Viewport(const std::string &title, int width, int height)
        : title(title), width(width), height(height) {}

    Viewport::~Viewport()
    {
        shutdown();
    }

    bool Viewport::initialize()
    {
        window = SDL_CreateWindow(title.c_str(), width, height, 0);
        if (!window)
        {
            std::cerr << "Failed to create SDL Window: " << SDL_GetError() << "\n";
            return false;
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer)
        {
            std::cerr << "Failed to create SDL Renderer: " << SDL_GetError() << "\n";
            return false;
        }

        return true;
    }

    void Viewport::shutdown()
    {
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }

        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
    }

    void Viewport::beginFrame()
    {
        if (!renderer)
            return;

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);
    }

    void Viewport::endFrame()
    {
        if (!renderer)
            return;

        SDL_RenderPresent(renderer);
    }

    bool Viewport::isValid() const
    {
        return window != nullptr && renderer != nullptr;
    }
}