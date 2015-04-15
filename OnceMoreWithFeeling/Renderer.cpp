#include "Renderer.h"
#include "Texture.h"

#include <sstream>
#include <iomanip>

using namespace OnceMoreWithFeeling;
using namespace std;

const string TEXT_VERTEX_SOURCE = "#version 410\n\
layout(location = 0) in vec2 vertex;\
layout(location = 1) in vec2 texCoord;\
layout(location = 0) out vec2 texCoordOut;\
uniform mat4 m;\
uniform mat4 v;\
uniform mat4 p;\
void main()\
{\
    texCoordOut = texCoord;\
    gl_Position = p * v * m * vec4(vertex, 0.0, 1.0);\
}";

const string TEXT_FRAGMENT_SOURCE = "#version 410\n\
layout(location = 0) in vec2 texCoord;\
layout(location = 0) out vec4 fragColour;\
uniform vec3 colour;\
uniform sampler2D font;\
void main()\
{\
    fragColour = vec4(colour, texture(font, texCoord).r);\
}";

vector<float> quadVerts = {
    0, 0, 0,
    0, 1, 0,
    1, 1, 0,
    1, 1, 0,
    1, 0, 0,
    0, 0, 0
};

vector<float> quadTexcoords = {
    0, 0,
    0, 1,
    1, 1,
    1, 1,
    1, 0,
    0, 0
};

Renderer::Renderer() : font_(), frameCount_(0), totalFrameCount_(0), fps_(0), 
    cameraPosition_(0, 0, 7), cameraLookAt_(0, 0, 0), cameraUp_(0, 1, 0),
    clearMask_(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
{
    shared_ptr<ShaderProgram> textProgram = make_shared<ShaderProgram>();
    textProgram->Build(TEXT_VERTEX_SOURCE, TEXT_FRAGMENT_SOURCE);
    shaders_.insert(make_pair("text|text", textProgram));
    
    shared_ptr<Buffer> vertices = make_shared<Buffer>();
    shared_ptr<Buffer> texCoords = make_shared<Buffer>();
    vertices->SetData(quadVerts);
    texCoords->SetData(quadTexcoords);

    quad_.AttachBuffer(0, vertices);
    quad_.AttachBuffer(1, texCoords, 2);
}

Renderer::~Renderer()
{
    for (auto shader : shaders_)
        glDeleteProgram(shader.second->Handle());
    for (auto texture : textures_)
        glDeleteTextures(1, &texture.second.second);
}

void Renderer::StartFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width_, height_);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    if (clearMask_ > 0)
        glClear(clearMask_);

    view_ = Matrix::Camera(cameraPosition_, cameraLookAt_, cameraUp_);
}

void Renderer::EndFrame()
{
    #ifndef OMWF_RECORDING
        stringstream ss;
        ss << "fps: " << fps_;
        DrawText(ss.str(), Vector(0, 5, 0));
    #endif

    #ifdef OMWF_RECORDING
        vector<unsigned char> framePixels(width_ * height_ * 4);
        glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, &framePixels[0]);
        ostringstream frameFilename;
        frameFilename << "f" << setfill('0') << setw(6) << totalFrameCount_ << ".png";
        SaveImage(frameFilename.str(), width_, height_, framePixels);
    #endif

    frameCount_++;
    totalFrameCount_ ++;
}

void Renderer::Draw(shared_ptr<RenderObject> renderObject, GLenum type)
{
    shared_ptr<ShaderProgram> program = shaders_[renderObject->program];
    program->Activate();

    GLint m = glGetUniformLocation(program->Handle(), "m");
    GLint v = glGetUniformLocation(program->Handle(), "v");
    GLint p = glGetUniformLocation(program->Handle(), "p");
    GLint c = glGetUniformLocation(program->Handle(), "colour");

    glUniformMatrix4fv(m, 1, GL_FALSE, renderObject->transformation.gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, view_.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, projection_.gl());
    glUniform3fv(c, 1, renderObject->colour);

    if (renderObject->textureBindings.size() > 0)
        SetTextures(renderObject->program, renderObject->textureBindings);

    renderObject->object->Draw(type);
}

void Renderer::DrawFullscreenQuad(string programName, unordered_map<unsigned int, string> textures)
{
    shared_ptr<ShaderProgram> program = shaders_[programName];
    program->Activate();

    GLint m = glGetUniformLocation(program->Handle(), "m");
    GLint v = glGetUniformLocation(program->Handle(), "v");
    GLint p = glGetUniformLocation(program->Handle(), "p");

    Matrix identity;
    glUniformMatrix4fv(m, 1, GL_FALSE, identity.gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, identity.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, Matrix::Ortho(1, 1).gl());

    if (textures.size() > 0)
        SetTextures(programName, textures);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    quad_.Draw(GL_TRIANGLES);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::DrawText(string text, Vector position, float size, Vector colour)
{
    vector<float> verts;
    vector<float> texCoords;
    font_.GetString(text, verts, texCoords);

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
    float c[] = { colour.x, colour.y, colour.z };

    Matrix identity;
    glUniformMatrix4fv(m, 1, GL_FALSE, (Matrix::Translate(position) * Matrix::Scale(size)).gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, identity.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, Matrix::Ortho(static_cast<float>(width_), static_cast<float>(height_)).gl());
    glUniform1i(t, 0);
    glUniform3fv(glGetUniformLocation(textProgram->Handle(), "colour"), 1, c);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_.GetTexture());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    o.Draw(GL_TRIANGLES);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::DrawText3d(string text, Vector position, float size, Vector colour)
{
    vector<float> verts;
    vector<float> texCoords;
    font_.GetString(text, verts, texCoords);

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
    float c[] = { colour.x, colour.y, colour.z };

    glUniformMatrix4fv(m, 1, GL_FALSE, (Matrix::Translate(position) * Matrix::Scale(size) * Matrix::Billboard(cameraPosition_, position)).gl());
    glUniformMatrix4fv(v, 1, GL_FALSE, view_.gl());
    glUniformMatrix4fv(p, 1, GL_FALSE, projection_.gl());
    glUniform1i(t, 0);
    glUniform3fv(glGetUniformLocation(textProgram->Handle(), "colour"), 1, c);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_.GetTexture());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    o.Draw(GL_TRIANGLES);

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

void Renderer::AddTexture(string textureName)
{
    GLuint handle = LoadTexture(textureName);
    AddTexture(textureName, handle);
}

void Renderer::AddTexture(string textureName, GLuint handle)
{
    // If a texture is replaced we don't delete the existing one, since we have
    // no way of knowing if it's used elsewhere.
    textures_[textureName] = make_pair(GL_TEXTURE_2D, handle);
}

void Renderer::AddCubeTexture(string textureName, vector<string> filenames)
{
    GLuint handle = LoadCubeTexture(filenames);
    AddTexture(textureName, handle);
}

GLuint Renderer::GetTexture(string textureName)
{
    auto t = textures_.find(textureName);
    if (t == textures_.end())
        return 0;
    return t->second.second;
}

void Renderer::SetWindowSize(unsigned int width, unsigned int height)
{
    width_ = width;
    height_ = height;
    projection_ = Matrix::Projection(0.1f, 50, static_cast<float>(height_) / width_, PI / 3);
}

void Renderer::SetUniform(string program, int location, int value)
{
    glProgramUniform1i(shaders_[program]->Handle(), location, value);
}

void Renderer::SetUniform(string program, int location, float value)
{
    glProgramUniform1f(shaders_[program]->Handle(), location, value);
}

void Renderer::SetUniform(string program, int location, Vector value)
{
    glProgramUniform3fv(shaders_[program]->Handle(), location, 1, value.gl());
}

void Renderer::SetUniform(string program, int location, Matrix value)
{
    glProgramUniformMatrix4fv(shaders_[program]->Handle(), location, 1, GL_FALSE, value.gl());
}

void Renderer::SetTextures(string program, unordered_map<unsigned int, string> bindings)
{
    auto shader = shaders_[program];
    int i = 0;
    for (auto binding : bindings)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(textures_[binding.second].first, textures_[binding.second].second);
        glProgramUniform1i(shader->Handle(), binding.first, i);
        i++;
    }
}

void Renderer::SetFramebuffer(shared_ptr<Framebuffer> framebuffer)
{
    framebuffer->Activate();
    unsigned int width = framebuffer->Width();
    unsigned int height = framebuffer->Height();
    glViewport(0, 0, width, height);
    projection_ = Matrix::Projection(0.1f, 50, static_cast<float>(height) / width, PI / 3);
}

void Renderer::ResetFramebuffer()
{
    Framebuffer::Deactivate();
    glViewport(0, 0, width_, height_);
    projection_ = Matrix::Projection(0.1f, 50, static_cast<float>(height_) / width_, PI / 3);
}
