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
                        -0.5f,  -0.5f,  0.5f,

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
    vector<Vector> rotation_;   // Yep, that won't get confusing
    vector<Vector> position_;
    vector<shared_ptr<RenderObject>> teapots_;
    shared_ptr<RenderObject> cube_;
    GLuint environmentTexture_;
};

void TeapotWorld::Init(shared_ptr<Renderer> renderer)
{
    renderer->AddShader("full", "full");
    renderer->AddShader("basic", "basic");
    renderer->AddShader("skybox", "skybox");

    shared_ptr<Buffer> cubeVertexBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> cubeNormalBuffer = make_shared<Buffer>();

    for (int i = 0; i < 108; ++i)
        cubeNormals[i] *= -1;

    cubeVertexBuffer->SetData(cubeVerts, 108);
    cubeNormalBuffer->SetData(cubeNormals, 108);

    shared_ptr<Object> c = make_shared<Object>();
    c->AttachBuffer(0, cubeVertexBuffer);
    c->AttachBuffer(1, cubeNormalBuffer);

    cube_ = make_shared<RenderObject>();
    cube_->object = c;
    cube_->program = "skybox|skybox"; // "basic|basic";
    cube_->transformation = Matrix::Scale(30, 12, 30);
    cube_->colour[0] = cube_->colour[1] = cube_->colour[2] = 1;
    cube_->specular = 0;
    cube_->shininess = 1;
    cube_->reflectiveness = 1;

    shared_ptr<Buffer> teapotVertexBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> teapotNormalBuffer = make_shared<Buffer>();

    teapotVertexBuffer->SetData(teapot_verts, 22176);
    teapotNormalBuffer->SetData(teapot_normals, 22176);

    shared_ptr<Object> o = make_shared<Object>();
    o->AttachBuffer(0, teapotVertexBuffer);
    o->AttachBuffer(1, teapotNormalBuffer);

    for (unsigned int i = 0; i < 50; ++i)
    {
        auto teapot = make_shared<RenderObject>();
        teapot->object = o;
        teapot->program = "full|full";
        teapot->colour[0] = RandF(0, 1);
        teapot->colour[1] = RandF(0, 1);
        teapot->colour[2] = RandF(0, 1);
        teapot->specular = 1;
        teapot->shininess = 128;
        teapot->reflectiveness = RandF(0, 1);
        teapots_.push_back(teapot);

        rotation_.push_back(Vector(RandF(0, PI), RandF(0, PI), RandF(0, PI)));
        position_.push_back(Vector(RandF(-5, 5), RandF(-2, 2), RandF(-5, 5)));
    }

    vector<string> environment = { "posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg" };
    environmentTexture_ = LoadCubeTexture(environment);
}

void TeapotWorld::Upate(float msecs)
{
    for (unsigned int i = 0; i < teapots_.size(); ++i)
    {
        rotation_[i].x -= 0.0002f * msecs;
        if (rotation_[i].x < 0)
            rotation_[i].x += PI * 2;

        rotation_[i].y -= 0.001f * msecs;
        if (rotation_[i].y < 0)
            rotation_[i].y += PI * 2;

        rotation_[i].z -= 0.0003f * msecs;
        if (rotation_[i].z < 0)
            rotation_[i].z += PI * 2;

        teapots_[i]->transformation = Matrix::Translate(position_[i]) * Matrix::Rotate(rotation_[i].x, rotation_[i].y, rotation_[i].z) * Matrix::Translate(Vector(0, -0.5f, 0));
    }
}

void TeapotWorld::Draw(shared_ptr<Renderer> renderer)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, environmentTexture_);

    renderer->Draw(cube_);

    for (auto teapot : teapots_)
        renderer->Draw(teapot);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<TeapotWorld>();

    world->Init(renderer);
    return w.Loop(world, renderer);
}