#include "ShaderProgram.h"

#include <fstream>
#include <sstream>

using namespace OnceMoreWithFeeling;
using namespace std;

bool ShaderProgram::Load(string vertexShaderName, string fragmentShaderName)
{
    ifstream vertexShaderFile(vertexShaderName + ".vert");
    bool derp = vertexShaderFile.fail();
    stringstream vertexShaderSource;
    vertexShaderSource << vertexShaderFile.rdbuf();
    vertexShaderFile.close();

    ifstream fragmentShaderFile(fragmentShaderName + ".frag");
    bool derp2 = fragmentShaderFile.fail();
    stringstream fragmentShaderSource;
    fragmentShaderSource << fragmentShaderFile.rdbuf();
    fragmentShaderFile.close();
    
    return Build(vertexShaderSource.str(), fragmentShaderSource.str());
}

bool ShaderProgram::Build(string vertexShaderSource, string fragmentShaderSource)
{
    GLint success = 0;
    char logBuffer[4096];
    auto vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vs = vertexShaderSource.c_str();
    const char *fs = fragmentShaderSource.c_str();

    glShaderSource(vertexShaderObject, 1, &vs, nullptr);
    glCompileShader(vertexShaderObject);
    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GetLog(vertexShaderObject, logBuffer, sizeof(logBuffer));
    }
    
    glShaderSource(fragmentShaderObject, 1, &fs, nullptr);
    glCompileShader(fragmentShaderObject);
    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GetLog(fragmentShaderObject, logBuffer, sizeof(logBuffer));
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vertexShaderObject);
    glAttachShader(program_, fragmentShaderObject);
    glLinkProgram(program_);
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GetLog(program_, logBuffer, sizeof(logBuffer));
    }

    return true;
}

void ShaderProgram::GetLog(GLuint handle, char *buffer, int bufferSize)
{
    glGetShaderInfoLog(handle, bufferSize, &bufferSize, buffer);
    glGetProgramInfoLog(handle, bufferSize, &bufferSize, buffer);
}

void ShaderProgram::Activate()
{
    glUseProgram(program_);
}

void ShaderProgram::Deactivate()
{
    glUseProgram(0);
}
