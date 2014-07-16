#pragma once

#include "glew.h"

#include <vector>

namespace OnceMoreWithFeeling
{
    struct Glyph
    {
        float xoffset;
        float yoffset;
        float width;
        float height;
        
        Glyph(float x, float y, float w, float h) : xoffset(x), yoffset(y), width(w), height(h) { }
    };

    class Font
    {
    public:
        Font();
        ~Font();
    
        void GetString(std::string s, std::vector<float> &verts, std::vector<float> &texCoords);
        GLuint GetTexture() { return texture_; }
        
    protected:
        std::vector<Glyph> glyphs_;
        GLuint texture_;
        unsigned int textureWidth_;
        unsigned int textureHeight_;
    };
}
