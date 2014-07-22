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
        float baseline;
        
        Glyph(float x, float y, float w, float h, float b) : xoffset(x), yoffset(y), width(w), height(h), baseline(b) { }
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
