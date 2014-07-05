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

void BuildWorld(shared_ptr<Renderer> renderer)
{
    shared_ptr<Buffer> vertexBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> normalBuffer = make_shared<Buffer>();
    
    vertexBuffer->SetData(cubeVerts, 18);
    normalBuffer->SetData(cubeNormals, 18);

    shared_ptr<Object> object = make_shared<Object>();
    object->AttachBuffer(0, vertexBuffer);
    object->AttachBuffer(1, normalBuffer);

    renderer->AddObject(object);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    BuildWorld(renderer);
    return w.Loop(renderer);
}
