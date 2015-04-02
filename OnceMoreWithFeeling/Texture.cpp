#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Lib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Lib/stb_image_write.h"

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

    glTexStorage2D(GL_TEXTURE_2D, 4, internalFormat, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

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

        if (i == 0)
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, internalFormat, width, height);

        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    return handle;
}

void OnceMoreWithFeeling::SaveImage(string filename, unsigned int width, unsigned int height, vector<unsigned char> &data)
{
    int result = stbi_write_png(filename.c_str(), width, height, 4, &data[0], 0);
}
