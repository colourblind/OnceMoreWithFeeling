#include <Windows.h>
#include <memory>
#include "Window.h"
#include "Renderer.h"
#include "World.h"
#include "Texture.h"

using namespace OnceMoreWithFeeling;
using namespace std;

float cubeVerts[] = {   -0.5f,  0.5f,   0.5f,
                        -0.5f,  -0.5f,  0.5f,
                        -0.5f,  -0.5f,  -0.5f,
                        -0.5f,  -0.5f,  -0.5f,
                        -0.5f,  0.5f,   -0.5f,
                        -0.5f,  0.5f,   0.5f,

                        0.5f,   -0.5f,  -0.5f,
                        0.5f,   -0.5f,  0.5f,
                        0.5f,   0.5f,   0.5f,
                        0.5f,   0.5f,   0.5f,
                        0.5f,   0.5f,   -0.5f,
                        0.5f,   -0.5f,  -0.5f,

                        0.5f,   0.5f,   -0.5f,
                        -0.5f,  0.5f,   -0.5f,
                        -0.5f,  -0.5f,  -0.5f,
                        -0.5f,  -0.5f,  -0.5f,
                        0.5f,   -0.5f,  -0.5f,
                        0.5f,   0.5f,   -0.5f,

                        -0.5f,  -0.5f,  0.5f,
                        -0.5f,  0.5f,   0.5f,
                        0.5f,   0.5f,   0.5f,
                        0.5f,   0.5f,   0.5f,
                        0.5f,   -0.5f,  0.5f,
                        -0.5f,  -0.5f,  0.5f

};

float cubeNormals[] = { -1, 0, 0,
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
};

float planeVerts[] = { 
    -20, -20, 0,
    -20, 20, 0,
    20, 20, 0,
    20, 20, 0,
    20, -20, 0,
    -20, -20, 0
};

float planeNormals[] = {
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
};

#include "wt_teapot.obj.h"

class TeapotWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    Vector rotation_;
    Vector position_;
    shared_ptr<RenderObject> teapot_;
    GLuint cube_;
};

void TeapotWorld::Init(shared_ptr<Renderer> renderer)
{
    shared_ptr<Buffer> teapotVertexBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> teapotNormalBuffer = make_shared<Buffer>();

    teapotVertexBuffer->SetData(teapot_verts, 22176);
    teapotNormalBuffer->SetData(teapot_normals, 22176);

    shared_ptr<Object> o = make_shared<Object>();
    o->AttachBuffer(0, teapotVertexBuffer);
    o->AttachBuffer(1, teapotNormalBuffer);

    teapot_ = make_shared<RenderObject>();
    teapot_->object = o;
    teapot_->program = "basic|basic";
    teapot_->colour[0] = 1.f;
    teapot_->colour[1] = 1.f;
    teapot_->colour[2] = 0;
    teapot_->shininess = 128;

    vector<string> environment = { "posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg" };
    cube_ = LoadCubeTexture(environment);

    position_.y = -0.5f;
}

void TeapotWorld::Upate(float msecs)
{
    rotation_.x -= 0.0002f * msecs;
    if (rotation_.x < 0)
        rotation_.x += PI * 2;

    rotation_.y -= 0.001f * msecs;
    if (rotation_.y < 0)
        rotation_.y += PI * 2;

    rotation_.z -= 0.0003f * msecs;
    if (rotation_.z < 0)
        rotation_.z += PI * 2;

    teapot_->transformation = Matrix::Rotate(rotation_.x, rotation_.y, rotation_.z) * Matrix::Translate(position_);
}

void TeapotWorld::Draw(shared_ptr<Renderer> renderer)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_);

    renderer->Draw(teapot_);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<TeapotWorld>();

    world->Init(renderer);
    return w.Loop(world, renderer);
}
