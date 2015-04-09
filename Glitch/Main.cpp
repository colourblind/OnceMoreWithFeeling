#include "Window.h"
#include "Renderer.h"
#include "World.h"
#include "Utils.h"
#include "Texture.h"

using namespace OnceMoreWithFeeling;
using namespace std;

vector<float> cubeVerts = {
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,

    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f
};

vector<float> cubeNormals = {
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,

    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,

    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,

    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,

    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
};

struct Glitch
{
    Matrix transformation;
};

class GlitchWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    shared_ptr<RenderObject> cube_;
    shared_ptr<Framebuffer> fbo_;

    unordered_map<unsigned int, string> textureBindings_;
    Quaternion view_;
    Vector cameraPosition_;
    vector<Glitch> glitches_;
};

void GlitchWorld::Init(shared_ptr<Renderer> renderer)
{
    unsigned int width, height;
    renderer->GetWindowSize(width, height);

    renderer->AddShader("basic", "basic");
    renderer->AddShader("final", "final");
    renderer->AddShader("final", "radial");

    fbo_ = make_shared<Framebuffer>(width, height);

    renderer->AddTexture("fbo_colour", fbo_->GetTexture());
    renderer->AddTexture("fbo_depth", fbo_->GetDepth());

    auto vertBuffer = make_shared<Buffer>();
    vertBuffer->SetData(cubeVerts);
    auto normBuffer = make_shared<Buffer>();
    normBuffer->SetData(cubeNormals);
    auto object = make_shared<Object>();
    object->AttachBuffer(0, vertBuffer);
    object->AttachBuffer(1, normBuffer);

    cube_ = make_shared<RenderObject>();
    cube_->object = object;
    cube_->program = "basic|basic";
    cube_->colour[0] = cube_->colour[1] = cube_->colour[2] = 0.75f;

    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    cameraPosition_ = Vector(0, 0, 5);

    renderer->SetClearMask(0);

    vector<float> noise0;
    CreateNoise(noise0, 5, 512, 512);
    GLuint noiseTexture0 = CreateTexture(noise0, 512, 512, 1);
    renderer->AddTexture("noise0", noiseTexture0);

    vector<float> noise1;
    CreateNoise(noise1, 5, 512, 512, 8);
    GLuint noiseTexture1 = CreateTexture(noise1, 512, 512, 1);
    renderer->AddTexture("noise1", noiseTexture1);

    for (int i = 0; i < 100; i++)
    {
        Glitch g;
        g.transformation = Matrix::Rotate(0, RandF(0, 2 * PI), 0) * Matrix::Translate(RandF(-10, 10), RandF(-10, 10), RandF(-10, 10)) * Matrix::Scale(RandF(0.25f, 4), RandF(0.25f, 4), RandF(0.25f, 4));
        glitches_.push_back(g);
    }

    textureBindings_.insert(make_pair(0, "fbo_colour"));
    textureBindings_.insert(make_pair(1, "fbo_depth"));
    textureBindings_.insert(make_pair(2, "noise0"));
    textureBindings_.insert(make_pair(3, "noise1"));
    textureBindings_.insert(make_pair(4, "glow"));

    
    // *************************************************

    GLuint glowAttachment;
    glGenTextures(1, &glowAttachment);
    glBindTexture(GL_TEXTURE_2D, glowAttachment);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);

    fbo_->Activate();
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, glowAttachment, 0);
    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, buffers);
    fbo_->Deactivate();

    renderer->AddTexture("glow", glowAttachment);

    // ************************************************


    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GlitchWorld::Upate(float msecs)
{
    float x, y;
    Input input;
    input.GetMouseOffset(&x, &y);

    Quaternion viewChange = Quaternion::FromEuler(y * 0.0025f, x * -0.0025f, 0);
    view_ = view_ * viewChange;
    view_.Normalise();

    Vector forward = view_ * Vector(0, 0, -1);
    Vector right = view_ * Vector(1, 0, 0);

    if (input.GetState(0x57)) // W
        cameraPosition_ = cameraPosition_ + forward * 0.01f * msecs;
    if (input.GetState(0x53)) // S
        cameraPosition_ = cameraPosition_ - forward * 0.01f * msecs;
    if (input.GetState(0x41)) // A
        cameraPosition_ = cameraPosition_ - right * 0.01f * msecs;
    if (input.GetState(0x44)) // D
        cameraPosition_ = cameraPosition_ + right * 0.01f * msecs;
}

void GlitchWorld::Draw(shared_ptr<Renderer> renderer)
{
    Vector cameraPosition = cameraPosition_ + Vector(RandF(-0.0025f, 0.0025f), RandF(-0.0025f, 0.0025f), RandF(-0.0025f, 0.0025f));

    renderer->SetCameraPosition(cameraPosition);
    Matrix viewDirection = view_.ToMatrix();
    renderer->SetCameraLookAt(cameraPosition_ + (viewDirection * Vector(0, 0, -1)));

    renderer->SetFramebuffer(fbo_);

    renderer->SetUniform("basic|basic", 4, viewDirection);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto g : glitches_)
    {
        cube_->transformation = g.transformation;
        renderer->Draw(cube_);
    }

    renderer->ResetFramebuffer();

    glEnable(GL_BLEND);

    renderer->SetUniform("final|final", 5, viewDirection);
    renderer->SetUniform("final|radial", 5, viewDirection);

    renderer->DrawFullscreenQuad("final|final", textureBindings_);
    renderer->DrawFullscreenQuad("final|radial", textureBindings_);

}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<GlitchWorld>();

    return w.Loop(world, renderer);
}
