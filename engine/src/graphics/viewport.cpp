#include "viewport.h"

#include "engine.h"

#include <iostream>

namespace Engine::Graphics
{
    Viewport::Viewport(int width, int height) : width(width), height(height) {}
    Viewport::~Viewport() { shutdown(); }

    bool Viewport::initialize()
    {
        // Create FBO
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Create texture attachment
        glGenTextures(1, &color_texture);
        glBindTexture(GL_TEXTURE_2D, color_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

        glGenRenderbuffers(1, &depth_rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);

        // Check framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Logger::log_error("[Viewport] Failed to create framebuffer for viewport");
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        valid = true;
        return true;
    }

    void Viewport::shutdown()
    {
        if (depth_rbo)
            glDeleteRenderbuffers(1, &depth_rbo);

        if (color_texture)
            glDeleteTextures(1, &color_texture);

        if (fbo)
            glDeleteFramebuffers(1, &fbo);
    }

    void Viewport::begin_frame()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Viewport::end_frame()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool Viewport::is_valid() const
    {
        return valid;
    }
}