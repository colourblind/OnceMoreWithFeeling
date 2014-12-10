#include "Window.h"
#include "Renderer.h"
#include "World.h"

#include "Framebuffer.h"

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

vector<float> cubeTexCoords = {
    1, 1,
    0, 1,
    0, 0,
    0, 0, 
    1, 0, 
    1, 1,

    0, 0,
    0, 1,
    1, 1,
    1, 1,
    1, 0,
    0, 0,

    1, 1,
    0, 1,
    0, 0,
    0, 0,
    1, 0,
    1, 1,

    0, 0,
    0, 1,
    1, 1,
    1, 1,
    1, 0,
    0, 0,

    0, 0,
    0, 1,
    1, 1,
    1, 1,
    1, 0,
    0, 0,

    1, 1,
    0, 1,
    0, 0,
    0, 0,
    1, 0,
    1, 1
};

class FboTestWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    shared_ptr<Framebuffer> framebuffer_;
    shared_ptr<RenderObject> cube_;
    shared_ptr<RenderObject> innerCube_;
    float rotate_;
};

void FboTestWorld::Init(shared_ptr<Renderer> renderer)
{
    renderer->AddShader("basic", "basic");
    renderer->AddShader("textured", "textured");
    
    framebuffer_ = make_shared<Framebuffer>(512, 512);

    renderer->AddTexture("fbo", framebuffer_->GetTexture());

    shared_ptr<Buffer> vertBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> texCoordBuffer = make_shared<Buffer>();
    vertBuffer->SetData(cubeVerts);
    texCoordBuffer->SetData(cubeTexCoords);

    shared_ptr<Object> o = make_shared<Object>();
    o->AttachBuffer(0, vertBuffer);
    o->AttachBuffer(1, texCoordBuffer, 2);
    
    cube_ = make_shared<RenderObject>();
    cube_->object = o;
    cube_->textureBindings.insert(make_pair(0, "fbo"));
    cube_->colour[0] = 1;
    cube_->colour[1] = 1;
    cube_->colour[2] = 1;
    cube_->program = "textured|textured";

    innerCube_ = make_shared<RenderObject>();
    innerCube_->object = o;
    innerCube_->colour[0] = 1;
    innerCube_->colour[1] = 0;
    innerCube_->colour[2] = 1;
    innerCube_->program = "basic|basic";
    
    rotate_ = 0;

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void FboTestWorld::Upate(float msecs)
{
    rotate_ += 0.001f * msecs;
    if (rotate_ > PI * 2)
        rotate_ -= PI * 2;

    cube_->transformation = Matrix::Rotate(0, rotate_, 0) * Matrix::Scale(3);
    innerCube_->transformation = Matrix::Rotate(0, rotate_, 0) * Matrix::Scale(3);
}

void FboTestWorld::Draw(shared_ptr<Renderer> renderer)
{
    renderer->SetFramebuffer(framebuffer_);

    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->Draw(innerCube_);

    renderer->ResetFramebuffer();

    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->Draw(cube_);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<FboTestWorld>();

    world->Init(renderer);
    return w.Loop(world, renderer);
}
