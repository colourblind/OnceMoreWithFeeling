#include <Windows.h>
#include <memory>
#include "Window.h"
#include "Renderer.h"

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

void BuildWorld(shared_ptr<Renderer> renderer)
{
    shared_ptr<Buffer> teapotVertexBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> teapotNormalBuffer = make_shared<Buffer>();

    teapotVertexBuffer->SetData(teapot_verts, 22176);
    teapotNormalBuffer->SetData(teapot_normals, 22176);

    shared_ptr<Object> teapot = make_shared<Object>();
    teapot->AttachBuffer(0, teapotVertexBuffer);
    teapot->AttachBuffer(1, teapotNormalBuffer);

    shared_ptr<RenderObject> o = make_shared<RenderObject>();
    o->object = teapot;
    o->program = "basic|basic";
    o->colour[0] = 1.f;
    o->colour[1] = 1.f;
    o->colour[2] = 0;
    o->shininess = 128;
    o->transformation = Matrix::Rotate(0, PI / 6, 0);

    renderer->AddObject(o);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    BuildWorld(renderer);
    return w.Loop(renderer);
}
