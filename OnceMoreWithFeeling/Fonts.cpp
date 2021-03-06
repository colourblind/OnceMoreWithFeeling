#include "Fonts.h"
#include "FontData.h"

using namespace OnceMoreWithFeeling;
using namespace std;

Font::Font() : glyphs_(FONT_DATA, FONT_DATA + 96), textureWidth_(FONT_WIDTH), textureHeight_(FONT_HEIGHT)
{
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, textureWidth_, textureHeight_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth_, textureWidth_, GL_RED, GL_UNSIGNED_BYTE, FONT_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Font::~Font()
{
    glDeleteTextures(1, &texture_);
}

void Font::GetString(string s, vector<float> &verts, vector<float> &texCoords)
{
    float advance = 0;
    for (auto iter = begin(s); iter != end(s); ++iter)
    {
        unsigned short i = static_cast<short>(*iter) - 32;
        Glyph g = glyphs_[i];

        float foot = FONT_SIZE - g.height - g.baseline;
        float v[12] = {
            advance, foot,
            advance + g.width, foot,
            advance, foot + g.height,
            advance + g.width, foot,
            advance + g.width, foot + g.height,
            advance, foot + g.height
        };
        verts.insert(end(verts), v, v + 12);

        float t[12] = {
            g.xoffset / textureWidth_, (g.yoffset + g.height) / textureHeight_,
            (g.xoffset + g.width) / textureWidth_, (g.yoffset + g.height) / textureHeight_,
            g.xoffset / textureWidth_, g.yoffset / textureHeight_,
            (g.xoffset + g.width) / textureWidth_, (g.yoffset + g.height) / textureHeight_,
            (g.xoffset + g.width) / textureWidth_, g.yoffset / textureHeight_,
            g.xoffset / textureWidth_, g.yoffset / textureHeight_
        };
        texCoords.insert(end(texCoords), t, t + 12);

        advance += g.width;
    }
}
