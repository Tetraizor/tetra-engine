#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <optional>

namespace Engine
{
    class WindowInstance
    {
    public:
        WindowInstance();
        ~WindowInstance();

        bool create(const std::string &title, int width, int height, bool headless = false);

        void destroy();

        SDL_Window *getSDLWindow() const;
        SDL_Renderer *getSDLRenderer() const;

        int getWidth() const;
        int getHeight() const;
        const std::string &getTitle() const;
        bool isHeadless() const;
        bool isValid() const;

    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;

        std::string title;

        int width = 0;
        int height = 0;
        bool headless = false;
        bool valid = false;
    };
}