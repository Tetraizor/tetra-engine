#include "engine/platform/window.h"
#include <iostream>

#include "engine/engine.h"

namespace Engine::Platform
{
    Window::Window(const Config &config)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            Logger::log_error("[Window] SDL_Init failed: " + std::string(SDL_GetError()));
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
            Logger::log_error("[Window] SDL_CreateWindow failed: " + std::string(SDL_GetError()));
            throw std::runtime_error("Failed to create SDL window.");
        }

        gl_context = std::make_unique<Engine::Graphics::OpenGL::OpenGLContext>();
        if (!gl_context->initialize(sdl_window))
        {
            Logger::log_error("[Window] Could not initialize OpenGLContext");
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

            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            swap_buffers();
        }
    }

    void Window::swap_buffers()
    {
        if (gl_context)
            gl_context->swap_buffers();
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
