#pragma once

#include <string>

#include "Lib/glew.h"

namespace OnceMoreWithFeeling
{
    class ShaderProgram
    {
    public:
        bool Load(std::string vertexShaderName, std::string fragmentShaderName);
        bool Build(std::string vertexShaderSource, std::string fragmentShaderSource);
        void Activate();
        static void Deactivate();

        GLuint Handle() { return program_; }

    protected:
        void GetLog(GLuint handle, char *buffer, int bufferSize);

        GLuint program_;
    };
}
