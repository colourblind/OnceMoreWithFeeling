#pragma once

#include "Lib/glew.h"
#include <string>
#include <vector>

namespace OnceMoreWithFeeling
{
    GLuint LoadTexture(std::string filename, GLuint wrap = GL_CLAMP_TO_EDGE);
    GLuint LoadCubeTexture(std::vector<std::string> filenames);
    GLuint CreateTexture(std::vector<float> &data, unsigned int width, unsigned int height, unsigned int componentsPerPixel, GLuint wrap = GL_CLAMP_TO_EDGE);
    void SaveImage(std::string filename, unsigned int width, unsigned int height, std::vector<unsigned char> &data);
}
