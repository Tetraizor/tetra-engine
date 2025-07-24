// platform/window.h
#pragma once

#include <string>
#include <memory>

#include <SDL3/SDL.h>

struct SDL_Window;

namespace Engine::Platform
{
    class Window
    {
    public:
        struct Config
        {
            std::string title = "Tetra Engine";
            int width = 1280;
            int height = 720;
            bool resizable = true;
            bool fullscreen = false;
        };

        Window(const Config &config);
        ~Window();

        void set_title(const std::string &title);
        void poll_events();
        void swap_buffers();

        int get_width() const;
        int get_height() const;

        bool should_close() const;

    private:
        SDL_Window *sdl_window;
        bool close_requested = false;

        void handle_event(const SDL_Event &event);
    };
}
