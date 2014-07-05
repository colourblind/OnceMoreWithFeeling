#pragma once

#include <string>

#include "glew.h"

namespace OnceMoreWithFeeling
{
    class ShaderProgram
    {
    public:
        bool Load(std::string vertexShaderName, std::string fragmentShaderName);
        void Activate();
        static void Deactivate();

        GLuint Handle() { return program_; }

    protected:
        bool Build(std::string vertexShaderSource, std::string fragmentShaderSource);
        void GetLog(GLuint handle, char *buffer, int bufferSize);

        GLuint program_;
    };
}
