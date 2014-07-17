#include "Renderer.h"
#include "Maths.h"

#include <sstream>

using namespace OnceMoreWithFeeling;
using namespace std;
 
float PI = 3.14159265359f;

float rot = 0;

Vector cameraPos(0, 0.5, 2);
Vector cameraLookat(0, 0.5, 0);
Vector objectPos;

Renderer::Renderer() : font_()
{
    AddShader("text", "text");
}

Renderer::~Renderer()
{
    for (auto iter = begin(shaders_); iter != end(shaders_); ++iter)
        glDeleteProgram(iter->second->Handle());
}

void Renderer::Render(float msecs)
{
    shared_ptr<ShaderProgram> basicProgram = shaders_["basic|basic"];
    basicProgram->Activate();

    float aspectRatio = static_cast<float>(height_) / width_;
    float nearClip = 0.1f;
    float farClip = 20;
    float fov = PI / 3;

    rot -= 0.001f * msecs;
    //cameraPos.x += 0.0005f;
    //objectPos.x += 0.0001f * msecs;
    if (rot < 0)
        rot += PI * 2;

    Matrix model = Matrix::Translate(objectPos) * Matrix::Rotate(0, rot, 0);
    Matrix view = Matrix::Camera(cameraPos, cameraLookat);
    Matrix proj = Matrix::Projection(nearClip, farClip, aspectRatio, fov);
    
    float colour[] = { 1.0f, 1.0f, 0.0f };
    
    GLint m = glGetUniformLocation(basicProgram->Handle(), "m");
    GLint v = glGetUniformLocation(basicProgram->Handle(), "v");
    GLint p = glGetUniformLocation(basicProgram->Handle(), "p");
    GLint c = glGetUniformLocation(basicProgram->Handle(), "colour");
    GLint s = glGetUniformLocation(basicProgram->Handle(), "shininess");
    GLint e = glGetUniformLocation(basicProgram->Handle(), "eyePosition");

    glUniformMatrix4fv(m, 1, GL_FALSE, model.gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, view.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, proj.gl());
    glUniform3fv(c, 1, colour);
    glUniform3fv(e, 1, cameraPos.gl());
    glUniform1f(s, 128);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);

    for (auto iter = begin(objects_); iter != end(objects_); ++iter)
    {
        (*iter)->Draw();
    }

    vector<float> verts;
    vector<float> texCoords;

    stringstream ss;
    ss << "test: " << rot;
    font_.GetString(ss.str(), verts, texCoords);

    shared_ptr<Buffer> textVerts, textTexCoords;
    textVerts = make_shared<Buffer>();
    textVerts->SetData(verts);
    textTexCoords = make_shared<Buffer>();
    textTexCoords->SetData(texCoords);
    Object o;
    o.AttachBuffer(0, textVerts, 2);
    o.AttachBuffer(1, textTexCoords, 2);

    shared_ptr<ShaderProgram> textProgram = shaders_["text|text"];
    textProgram->Activate();

    m = glGetUniformLocation(textProgram->Handle(), "m");
    v = glGetUniformLocation(textProgram->Handle(), "v");
    p = glGetUniformLocation(textProgram->Handle(), "p");
    GLint t = glGetUniformLocation(textProgram->Handle(), "font");
    colour[0] = colour[1] = colour[2] = 1;

    Matrix identity;
    glUniformMatrix4fv(m, 1, GL_FALSE, identity.gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, identity.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, Matrix::Ortho(static_cast<float>(width_), static_cast<float>(height_)).gl());
    glUniform1i(t, 0);
    c = glGetUniformLocation(textProgram->Handle(), "colour");
    glUniform3fv(c, 1, colour);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_.GetTexture());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    o.Draw();

    glDisable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, 0);
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
