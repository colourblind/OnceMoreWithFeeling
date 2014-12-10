#pragma once

#include "Lib/glew.h"

namespace OnceMoreWithFeeling
{
    class Framebuffer
    {
    public:
        Framebuffer(unsigned int width, unsigned int height);
        ~Framebuffer();

        void Activate();
        static void Deactivate();

        GLuint GetTexture() const { return texture_; }
        unsigned int Width() const { return width_; }
        unsigned int Height() const { return height_; }

    private:
        GLuint handle_;
        GLuint texture_;
        GLuint depth_;
        unsigned int width_;
        unsigned int height_;
    };
}
