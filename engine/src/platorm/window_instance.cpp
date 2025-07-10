#include "platform/window_instance.h"
#include <iostream>

namespace Engine
{
    WindowInstance::WindowInstance() = default;
    WindowInstance::~WindowInstance() { destroy(); }

    bool WindowInstance::create(const std::string &title, int width, int height, bool headless)
    {
        this->title = title;
        this->width = width;
        this->height = height;
        this->headless = headless;

        if (headless)
        {
            this->valid = true;
        }

        window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
        if (!window)
        {
            return false;
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer)
        {
            SDL_DestroyWindow(window);
            return false;
        }

        valid = true;
        return true;
    }

    void WindowInstance::destroy()
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

        valid = false;
    }

    SDL_Window *WindowInstance::getSDLWindow() const
    {
        return window;
    }

    SDL_Renderer *WindowInstance::getSDLRenderer() const
    {
        return renderer;
    }

    int WindowInstance::getWidth() const
    {
        return width;
    }

    int WindowInstance::getHeight() const
    {
        return height;
    }

    const std::string &WindowInstance::getTitle() const
    {
        return title;
    }

    bool WindowInstance::isHeadless() const
    {
        return headless;
    }

    bool WindowInstance::isValid() const
    {
        return valid;
    }
}
