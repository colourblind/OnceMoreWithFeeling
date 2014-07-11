#include "Renderer.h"
#include "Maths.h"

using namespace OnceMoreWithFeeling;
using namespace std;
 
float PI = 3.14159265359f;

float rot = 0;

Vector cameraPos(0, 0, 6);
Vector cameraLookat;
Vector objectPos;

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

    float aspectRatio = static_cast<float>(height_) / width_;
    float nearClip = 1;
    float farClip = 20;
    float fov = PI / 3;

    //rot += 0.0001f;
    //cameraPos.x += 0.0005f;
    objectPos.x += 0.0005f;

    Matrix model = Matrix::Translate(objectPos) * Matrix::Rotate(0, rot, 0);
    Matrix view = Matrix::Camera(cameraPos, cameraLookat);
    Matrix proj;
    
    float s = 1.f / tan(fov / 2);
    proj.a[0][0] = aspectRatio * s;
    proj.a[1][1] = s;
    proj.a[2][2] = (farClip + nearClip) / (nearClip - farClip);
    proj.a[2][3] = -1;
    proj.a[3][2] = 2 * (farClip * nearClip) / (nearClip - farClip);
    proj.a[3][3] = 0;

    float colour[] = { 1.0f, 1.0f, 0.0f };
    
    GLint m = glGetUniformLocation(basicProgram->Handle(), "m");
    GLint v = glGetUniformLocation(basicProgram->Handle(), "v");
    GLint p = glGetUniformLocation(basicProgram->Handle(), "p");
    GLint c = glGetUniformLocation(basicProgram->Handle(), "colour");

    glUniformMatrix4fv(m, 1, GL_FALSE, model.gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, view.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, proj.gl());
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

void Renderer::SetWindowSize(unsigned int width, unsigned int height)
{
    width_ = width;
    height_ = height;
}
