#pragma once

#include "Lib/glew.h"
#include <string>
#include <vector>

namespace OnceMoreWithFeeling
{
    GLuint LoadTexture(std::string filename);
    GLuint LoadCubeTexture(std::vector<std::string> filenames);
}
