#include "platform/window.h"
#include <iostream>

namespace Engine::Platform
{
    Window::Window(const Config &config)
    {
        set_title(config.title);

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cerr << "[Window] SDL_Init failed: " << SDL_GetError() << std::endl;
            throw std::runtime_error("Failed to initialize SDL.");
        }

        Uint32 flags = SDL_WINDOW_VULKAN;

        if (config.resizable)
            flags |= SDL_WINDOW_RESIZABLE;
        if (config.fullscreen)
            flags |= SDL_WINDOW_FULLSCREEN;

        sdl_window = SDL_CreateWindow(config.title.c_str(), config.width, config.height, flags);
        if (!sdl_window)
        {
            std::cerr << "[Window] SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            throw std::runtime_error("Failed to create SDL window.");
        }
    }

    Window::~Window()
    {
        if (sdl_window)
        {
            SDL_DestroyWindow(sdl_window);
            sdl_window = nullptr;
        }

        SDL_Quit();
    }

    void Window::set_title(const std::string &title)
    {
        SDL_SetWindowTitle(sdl_window, title.c_str());
    }

    void Window::poll_events()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            handle_event(event);
        }
    }

    void Window::swap_buffers()
    {
        // TODO: Add a rendering context like Vulkan or OpenGL(?)
    }

    int Window::get_width() const
    {
        int w;
        SDL_GetWindowSize(sdl_window, &w, nullptr);
        return w;
    }

    int Window::get_height() const
    {
        int h;
        SDL_GetWindowSize(sdl_window, nullptr, &h);
        return h;
    }

    bool Window::should_close() const
    {
        return close_requested;
    }

    void Window::handle_event(const SDL_Event &event)
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            close_requested = true;
            break;

        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            if (event.window.windowID == SDL_GetWindowID(sdl_window))
                close_requested = true;
            break;

        default:
            // TODO: Handle remaining events, preferably in another module
            break;
        }
    }
}
