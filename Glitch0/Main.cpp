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

class Glitch0World : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    shared_ptr<RenderObject> glitch_;
    shared_ptr<Framebuffer> fbo_;
    shared_ptr<Framebuffer> frame_;

    unordered_map<unsigned int, string> textureBindings_;
    Vector yawPitchRoll_;
    Quaternion view_;
    Vector cameraPosition_;
    vector<float> noisePile_;
    float totalTime_;
    float tickTime_;
};

void Glitch0World::Init(shared_ptr<Renderer> renderer)
{
    // TODO: create 2 component perlin noise for smearing
    // TODO: make noise texture wrap

    unsigned int width, height;
    renderer->GetWindowSize(width, height);

    renderer->AddShader("basic", "basic");
    renderer->AddShader("final", "final");
    renderer->AddShader("final", "smear");
    renderer->AddShader("frame", "frame");

    fbo_ = make_shared<Framebuffer>(width, height);

    renderer->AddTexture("fbo_colour", fbo_->GetTexture());
    renderer->AddTexture("fbo_depth", fbo_->GetDepth());

    frame_ = make_shared<Framebuffer>(width, height);

    renderer->AddTexture("frame", frame_->GetTexture());

    vector<Vector> glitchVerts;
    vector<Vector> glitchNormals;
    vector<float> glitchTexCoords;

    for (int i = 0; i < 500; ++i)
    {
        Vector centre(RandF(-10, 10), RandF(-10, 10), RandF(-10, 10));
        Vector a = centre + Vector(RandF(-2, 2), RandF(-2, 2), RandF(-2, 2));
        Vector b = centre + Vector(RandF(-2, 2), RandF(-2, 2), RandF(-2, 2));
        Vector c = centre + Vector(RandF(-2, 2), RandF(-2, 2), RandF(-2, 2));
        glitchVerts.push_back(a);
        glitchVerts.push_back(b);
        glitchVerts.push_back(c);
        Vector normal = Vector::Cross(b - a, c - a).Normalised();
        glitchNormals.push_back(normal);
        glitchNormals.push_back(normal);
        glitchNormals.push_back(normal);
        glitchTexCoords.push_back(0.0);
        glitchTexCoords.push_back(0.0);
        glitchTexCoords.push_back(1.0);
        glitchTexCoords.push_back(0.0);
        glitchTexCoords.push_back(0.0);
        glitchTexCoords.push_back(1.0);
    }

    auto vertBuffer = make_shared<Buffer>();
    vertBuffer->SetData(glitchVerts);
    auto normBuffer = make_shared<Buffer>();
    normBuffer->SetData(glitchNormals);
    auto texCoordBuffer = make_shared<Buffer>();
    texCoordBuffer->SetData(glitchTexCoords);
    auto object = make_shared<Object>();
    object->AttachBuffer(0, vertBuffer);
    object->AttachBuffer(1, normBuffer);
    object->AttachBuffer(2, texCoordBuffer, 2);

    glitch_ = make_shared<RenderObject>();
    glitch_->object = object;
    glitch_->program = "basic|basic";
    glitch_->colour[0] = glitch_->colour[1] = glitch_->colour[2] = 0.75f;

    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

    cameraPosition_ = Vector(0, 0, 5);

    renderer->SetClearMask(0);

    vector<float> noise0;
    CreateShitNoise(noise0, 1024, 1024);
    GLuint noiseTexture0 = CreateTexture(noise0, 1024, 1024, 1, GL_REPEAT);
    renderer->AddTexture("noise0", noiseTexture0);

    vector<float> noise1r;
    CreateNoise(noise1r, 2, 1024, 1024, 16);
    vector<float> noise1g;
    CreateNoise(noise1g, 2, 1024, 1024, 16);
    vector<float> noise1;
    noise1.resize(noise1r.size() + noise1g.size());
    for (unsigned int i = 0; i < noise1r.size(); ++i)
    {
        noise1[i * 2] = noise1r[i];
        noise1[(noise1r.size() - i - 1) * 2 + 1] = noise1g[i];
    }

    GLuint noiseTexture1 = CreateTexture(noise1, 1024, 1024, 2, GL_REPEAT);
    renderer->AddTexture("noise1", noiseTexture1);

    textureBindings_.insert(make_pair(0, "fbo_colour"));
    textureBindings_.insert(make_pair(1, "fbo_depth"));
    textureBindings_.insert(make_pair(2, "noise0"));
    textureBindings_.insert(make_pair(3, "noise1"));
    textureBindings_.insert(make_pair(4, "last_frame"));

    glitch_->textureBindings = textureBindings_;
    
    // *************************************************

    GLuint saveAttachment;
    glGenTextures(1, &saveAttachment);
    glBindTexture(GL_TEXTURE_2D, saveAttachment);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);

    frame_->Activate();
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, saveAttachment, 0);
    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, buffers);
    frame_->Deactivate();

    renderer->AddTexture("last_frame", saveAttachment);

    // ************************************************

    CreateNoise(noisePile_, 12, 512, 1, 8);
    totalTime_ = 0;
    tickTime_ = 0;

    glClearColor(0.f, 0.f, 0.f, 0.05f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Glitch0World::Upate(float msecs)
{
    float x, y;
    Input input;
    input.GetMouseOffset(&x, &y);

    yawPitchRoll_ = yawPitchRoll_ + Vector(x * -0.0025f, y * 0.0025f, 0);
    Quaternion yaw(Vector(0, 1, 0), yawPitchRoll_.x);
    Quaternion pitch(Vector(1, 0, 0), yawPitchRoll_.y);
    view_ = yaw * pitch;
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

    totalTime_ += msecs / 100.0;
    if (totalTime_ > 512)
        totalTime_ -= 512;
    tickTime_ += msecs;
}

void Glitch0World::Draw(shared_ptr<Renderer> renderer)
{
    Vector cameraPosition = cameraPosition_ + Vector(RandF(-0.0025f, 0.0025f), RandF(-0.0025f, 0.0025f), RandF(-0.0025f, 0.0025f));

    renderer->SetCameraPosition(cameraPosition);
    Matrix viewDirection = view_.ToMatrix();
    renderer->SetCameraLookAt(cameraPosition_ + (viewDirection * Vector(0, 0, -1)));

    renderer->SetUniform("basic|basic", 4, viewDirection);
    renderer->SetUniform("final|final", 5, viewDirection);
    renderer->SetUniform("final|smear", 5, viewDirection);
    renderer->SetUniform("final|smear", 20, noisePile_[static_cast<int>(totalTime_)]);
    renderer->SetUniform("final|smear", 21, noisePile_[static_cast<int>(totalTime_) + 256 % 512]);
    renderer->SetUniform("final|smear", 22, tickTime_);

    renderer->SetFramebuffer(fbo_);
    glEnable(GL_BLEND);

    renderer->DrawFullscreenQuad("final|smear", textureBindings_);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderer->Draw(glitch_);

    renderer->SetFramebuffer(frame_);
    glEnable(GL_BLEND);

    renderer->DrawFullscreenQuad("final|final", textureBindings_);
    
    renderer->ResetFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT);

    unordered_map<unsigned int, string> t = {
        { 0, "frame" }
    };


    renderer->DrawFullscreenQuad("frame|frame", t);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<Glitch0World>();

    return w.Loop(world, renderer);
}
