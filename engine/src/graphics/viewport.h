#pragma once

#include <string>
#include <glad/glad.h>

namespace Engine::Graphics
{
    class Viewport
    {
    public:
        Viewport(int width, int height);
        ~Viewport();

        bool initialize();
        void shutdown();

        void begin_frame();
        void end_frame();

        bool is_valid() const;

    private:
        int width = 0;
        int height = 0;

        GLuint fbo = 0;
        GLuint color_texture = 0;
        GLuint depth_rbo = 0;

        bool valid = false;
    };
}