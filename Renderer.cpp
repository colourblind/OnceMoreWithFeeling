#include "Renderer.h"

#include <sstream>

using namespace OnceMoreWithFeeling;
using namespace std;

Vector cameraPos(0, 0, 2);
Vector cameraLookat(0, 0, 0);

Renderer::Renderer() : font_(), frameCount_(0), fps_(0)
{
    AddShader("text", "text");
}

Renderer::~Renderer()
{
    for (auto iter = begin(shaders_); iter != end(shaders_); ++iter)
        glDeleteProgram(iter->second->Handle());
}

void Renderer::StartFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width_, height_);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);

    float aspectRatio = static_cast<float>(height_) / width_;
    float nearClip = 0.1f;
    float farClip = 20;
    float fov = PI / 3;

    view_ = Matrix::Camera(cameraPos, cameraLookat);
    projection_ = Matrix::Projection(nearClip, farClip, aspectRatio, fov);
}

void Renderer::EndFrame()
{
    vector<float> verts;
    vector<float> texCoords;

    stringstream ss;
    ss << "fps: " << fps_;
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

    GLint m = glGetUniformLocation(textProgram->Handle(), "m");
    GLint v = glGetUniformLocation(textProgram->Handle(), "v");
    GLint p = glGetUniformLocation(textProgram->Handle(), "p");
    GLint t = glGetUniformLocation(textProgram->Handle(), "font");
    float colour[] = { 1.f, 1.f, 1.f };

    Matrix identity;
    glUniformMatrix4fv(m, 1, GL_FALSE, Matrix::Translate(0, 5, 0).gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, identity.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, Matrix::Ortho(static_cast<float>(width_), static_cast<float>(height_)).gl());
    glUniform1i(t, 0);
    GLint c = glGetUniformLocation(textProgram->Handle(), "colour");
    glUniform3fv(c, 1, colour);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_.GetTexture());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    o.Draw();

    glDisable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, 0);

    frameCount_++;
}

void Renderer::Draw(shared_ptr<RenderObject> renderObject)
{
    shared_ptr<ShaderProgram> program = shaders_[renderObject->program];
    program->Activate();

    GLint m = glGetUniformLocation(program->Handle(), "m");
    GLint v = glGetUniformLocation(program->Handle(), "v");
    GLint p = glGetUniformLocation(program->Handle(), "p");
    GLint c = glGetUniformLocation(program->Handle(), "colour");
    GLint s = glGetUniformLocation(program->Handle(), "shininess");
    GLint e = glGetUniformLocation(program->Handle(), "eyePosition");
    GLint environment = glGetUniformLocation(program->Handle(), "environment");

    glUniformMatrix4fv(m, 1, GL_FALSE, renderObject->transformation.gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, view_.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, projection_.gl());
    glUniform3fv(c, 1, renderObject->colour);
    glUniform1f(s, renderObject->shininess);
    glUniform3fv(e, 1, cameraPos.gl());

    glUniform1i(environment, 0);

    renderObject->object->Draw();
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
