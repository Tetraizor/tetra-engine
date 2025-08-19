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

        void resize(int width, int height);

        int get_width() { return width; }
        int get_height() { return height; }

        GLuint get_fbo() { return fbo; }
        GLuint get_color_texture() { return color_texture; }
        GLuint get_depth_rbo() { return depth_rbo; }

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