#include <Windows.h>
#include <memory>
#include "Window.h"
#include "Renderer.h"
#include "World.h"
#include "Texture.h"

using namespace OnceMoreWithFeeling;
using namespace std;

const unsigned int NUM_BOIDS = 1000;
const float NOTICE_DISTANCE = 0.75f;
const float OPTIMAL_DISTANCE = 0.25f;
const float MAX_SPEED = 0.001f;
const float RESPONSIVENESS = 0.000002f;
const float WING_BEAT_SPEED = 0.003f;

float boidVerts[] = {
    -0.2f, 0.0f, -0.4f,
    0.2f, 0.0f, -0.4f,
    0, 0, 0,
    0, 0, 0,
    -0.65f, 0.5f, 0.25f,
    0, 0, 0.5f,
    0, 0, 0,
    0.65f, 0.5f, 0.25f,
    0, 0, 0.5f
};

float boidNormals[] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
};

struct Boid
{
    Vector position;
    Vector velocity;
    Vector acceleration;
    shared_ptr<RenderObject> renderObject;
    float anim;
};

class SwarmWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    vector<shared_ptr<Boid>> boids_;
    float ermergerd_;
};

void SwarmWorld::Init(shared_ptr<Renderer> renderer)
{
    renderer->AddShader("basic", "basic");

    shared_ptr<Buffer> boidVertexBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> boidNormalBuffer = make_shared<Buffer>();

    boidVertexBuffer->SetData(boidVerts, 27);
    boidNormalBuffer->SetData(boidNormals, 27);

    shared_ptr<Object> o = make_shared<Object>();
    o->AttachBuffer(0, boidVertexBuffer);
    o->AttachBuffer(1, boidNormalBuffer);

    for (unsigned int i = 0; i < NUM_BOIDS; ++i)
    {
        shared_ptr<Boid> b = make_shared<Boid>();
        b->position = Vector(RandF(-4, 4), RandF(-4, 4), RandF(-4, 4));
        b->velocity = Vector(RandF(-0.01f, 0.01f), RandF(-0.01f, 0.01f), RandF(-0.01f, 0.01f));
        b->anim = RandF(0, 1);
        
        b->renderObject = make_shared<RenderObject>();
        b->renderObject->object = o;
        b->renderObject->program = "basic|basic";
        b->renderObject->colour[0] = b->renderObject->colour[1] = b->renderObject->colour[2] = 1.f;

        boids_.push_back(b);
    }

    ermergerd_ = 0;
}

void SwarmWorld::Upate(float msecs)
{
    // Freak out time!
    if (RandF(0, 1) > 0.998f)
        ermergerd_ = 250;

    for (unsigned int i = 0; i < boids_.size(); ++i)
    {
        shared_ptr<Boid> current = boids_[i];

        Vector cohesion;
        Vector seperation;
        Vector alignment;
        int neighbourCount = 1;
        for (unsigned int j = 0; j < boids_.size(); ++j)
        {
            if (i == j)
                continue;

            shared_ptr<Boid> b = boids_[j];
            Vector v = b->position - current->position;
            if (v.LengthSq() < NOTICE_DISTANCE * NOTICE_DISTANCE)
            {
                // cohesion and seperation
                if (v.LengthSq() > OPTIMAL_DISTANCE * OPTIMAL_DISTANCE)
                    cohesion = cohesion + v;
                else
                    seperation = seperation - v;
                // alignment
                alignment = alignment + b->velocity;
                neighbourCount ++;
            }
        }

        cohesion = cohesion * (1.f / neighbourCount);
        seperation = seperation * (1.f / neighbourCount);
        alignment = alignment * (1.f / neighbourCount);

        Vector cohesionPull = cohesion.Normalise() * 0.2f;
        Vector seperationPull = seperation.Normalise() * 1.5f;
        Vector alignmentPull = alignment.Normalise() * 0.5f;
        Vector centerPull = (current->position * -1) * 0.1f;

        if (ermergerd_ > 0)
            cohesionPull = cohesion * -2;

        Vector pull = cohesionPull + seperationPull + alignmentPull + centerPull;

        current->acceleration = pull * RESPONSIVENESS;
        current->velocity = current->velocity + current->acceleration * msecs;

        if (current->velocity.LengthSq() > MAX_SPEED * MAX_SPEED)
            current->velocity = current->velocity.Normalise() * MAX_SPEED;
        current->position = current->position + current->velocity * msecs;

        // Use our camera view transformation to align the 'mesh' to
        // its velocity vector
        Matrix align = Matrix::Camera(Vector(), current->velocity);
        current->renderObject->transformation = Matrix::Translate(current->position) * align * Matrix::Scale(0.1f);

        current->anim += WING_BEAT_SPEED * msecs;
        if (current->anim > 1)
            current->anim -= 1;
    }

    ermergerd_ -= msecs;
}

void SwarmWorld::Draw(shared_ptr<Renderer> renderer)
{
    for (auto b : boids_)
    {
        //if (ermergerd_ > 0)
        //    b->renderObject->colour[0] = b->renderObject->colour[1] = b->renderObject->colour[2] = 1;
        //else
            b->renderObject->colour[0] = b->renderObject->colour[1] = b->renderObject->colour[2] = 0;

        // TODO: get animation value into shaders :-/
        b->renderObject->colour[2] = b->anim;

        renderer->Draw(b->renderObject);
    }
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<SwarmWorld>();

    world->Init(renderer);
    return w.Loop(world, renderer);
}
