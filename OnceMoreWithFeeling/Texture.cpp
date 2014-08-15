#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Lib/stb_image.h"

using namespace OnceMoreWithFeeling;
using namespace std;

GLuint OnceMoreWithFeeling::LoadTexture(string filename)
{
    int width, height, componentsPerPixel;

    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &componentsPerPixel, 0);

    GLenum internalFormat = GL_RGB8;
    GLenum format = GL_RGB;
    switch (componentsPerPixel)
    {
    case 4:
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    return handle;
}

GLuint OnceMoreWithFeeling::LoadCubeTexture(vector<string> filenames)
{
    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, componentsPerPixel;
    for (int i = 0; i < 6; ++i)
    {
        unsigned char *data = stbi_load(filenames[i].c_str(), &width, &height, &componentsPerPixel, 0);

        GLenum internalFormat = GL_RGB8;
        GLenum format = GL_RGB;
        switch (componentsPerPixel)
        {
        case 4:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            break;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    return handle;
}
