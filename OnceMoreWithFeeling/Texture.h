#pragma once

#include "Lib/glew.h"
#include <string>
#include <vector>

namespace OnceMoreWithFeeling
{
    GLuint LoadTexture(std::string filename);
    GLuint LoadCubeTexture(std::vector<std::string> filenames);
    void SaveImage(std::string filename, unsigned int width, unsigned int height, std::vector<unsigned char> &data);
}
