#include "Renderer.h"

using namespace OnceMoreWithFeeling;
using namespace std;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
    for (auto iter = begin(shaders_); iter != end(shaders_); ++iter)
        glDeleteProgram(iter->second->Handle());
}

void Renderer::Render()
{
    shared_ptr<ShaderProgram> basicProgram = shaders_["basic|basic"];
    basicProgram->Activate();

    float model[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f 
    };

    float view[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float proj[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float colour[] = { 1.0f, 1.0f, 0.0f };
    
    GLint m = glGetUniformLocation(basicProgram->Handle(), "m");
    GLint v = glGetUniformLocation(basicProgram->Handle(), "v");
    GLint p = glGetUniformLocation(basicProgram->Handle(), "p");
    GLint c = glGetUniformLocation(basicProgram->Handle(), "colour");

    glUniformMatrix4fv(m, 1, GL_FALSE, model);
    glUniformMatrix4fv(v, 1, GL_FALSE, view);
    glUniformMatrix4fv(p, 1, GL_FALSE, proj);
    glUniform3fv(c, 1, colour);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);

    for (auto iter = begin(objects_); iter != end(objects_); ++iter)
    {
        (*iter)->Draw();
    }
}

void Renderer::AddShader(string vertexShaderName, string fragmentShaderName)
{
    shared_ptr<ShaderProgram> program = make_shared<ShaderProgram>();
    program->Load(vertexShaderName, fragmentShaderName);

    string name = vertexShaderName + '|' + fragmentShaderName;
    shaders_.insert(make_pair(name, program));
}
