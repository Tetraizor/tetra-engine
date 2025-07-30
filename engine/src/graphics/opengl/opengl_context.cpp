#include "graphics/opengl/opengl_context.h"

#include <iostream>

namespace Engine::Graphics::OpenGL
{
    OpenGLContext::~OpenGLContext()
    {
        if (gl_context)
        {
            SDL_GL_DestroyContext(gl_context);
            gl_context = nullptr;
        }
    }

    bool OpenGLContext::initialize(SDL_Window *window)
    {
        if (!window)
        {
            std::cerr << "[OpenGLContext] Invalid SDL_Window pointer!" << std::endl;
            return false;
        }

        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
        {
            std::cerr << "[OpenGLContext] Failed to set context attributes: " << SDL_GetError() << std::endl;
            return false;
        }

        gl_context = SDL_GL_CreateContext(window);
        if (!gl_context)
        {
            std::cerr << "[OpenGLContext] Failed to create OpenGL context: " << SDL_GetError() << std::endl;
            return false;
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            std::cerr << "[OpenGLContext] Failed to initialize GLAD." << std::endl;
            return false;
        }

        std::cout << "[OpenGLContext] OpenGL context initialized successfully." << std::endl;
        std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;

        return true;
    }

    void OpenGLContext::swap_buffers()
    {
        SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
    }
}