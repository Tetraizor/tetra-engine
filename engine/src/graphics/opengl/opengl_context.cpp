#include "engine/graphics/opengl/opengl_context.h"

#include "engine/debug/logging/logger.h"

#include <iostream>
#include <string>

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
            Logger::log_error("[OpenGLContext] Invalid SDL_Window pointer");
            return false;
        }

        if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5) < 0 ||
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
        {
            Logger::log_error("[OpenGLContext] Failed to set context attributes: " + std::string(SDL_GetError()));
            return false;
        }

        gl_context = SDL_GL_CreateContext(window);
        if (!gl_context)
        {
            Logger::log_error("[OpenGLContext] Failed to create OpenGL context: " + std::string(SDL_GetError()));
            return false;
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            Logger::log_error("[OpenGLContext] Failed to initialize GLAD.");
            return false;
        }

        Logger::log_info("[OpenGLContext] OpenGL context initialized successfully");
        Logger::log_info(std::string("  Version: ") + reinterpret_cast<const char *>(glGetString(GL_VERSION)));
        Logger::log_info(std::string("  Renderer: ") + reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        Logger::log_info(std::string("  Vendor: ") + reinterpret_cast<const char *>(glGetString(GL_VENDOR)));

        return true;
    }

    void OpenGLContext::swap_buffers()
    {
        SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
    }
}
