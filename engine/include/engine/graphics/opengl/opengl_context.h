#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace Engine::Graphics::OpenGL
{
    class OpenGLContext
    {
    public:
        OpenGLContext() = default;
        ~OpenGLContext();

        bool initialize(SDL_Window *window);

        void swap_buffers();

    private:
        SDL_GLContext gl_context = nullptr;
    };
}