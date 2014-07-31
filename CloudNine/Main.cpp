#include <Windows.h>
#include <algorithm>
#include "Window.h"
#include "Renderer.h"
#include "World.h"
#include "Texture.h"
#include "Noise.h"

using namespace OnceMoreWithFeeling;
using namespace std;

const int CLOUDS = 20;
const int FLUFFS_PER_CLOUD = 40;
const float LIGHTNING_TIME = 1000;

float planeVerts[] = { 
    -0.5f, -0.5f, 0,
    -0.5f, 0.5f, 0,
    0.5f, 0.5f, 0,
    0.5f, 0.5f, 0,
    0.5f, -0.5f, 0,
    -0.5f, -0.5f, 0
};

float planeTexCoords[] = {
    0, 0,
    0, 1,
    1, 1,
    1, 1,
    1, 0,
    0, 0
};

class CloudNineWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    vector<shared_ptr<RenderObject>> fluffs_; 
    vector<Vector> fluffNormals_;
    GLuint cloudTexture_;
    float flashTime_;
    Vector flashPosition_;
    float lightRotation_;
};

void CloudNineWorld::Init(shared_ptr<Renderer> renderer)
{
    renderer->AddShader("fluff", "fluff");

    auto planeVertexBuffer = make_shared<Buffer>();
    auto planeTexCoordBuffer = make_shared<Buffer>();

    planeVertexBuffer->SetData(planeVerts, 18);
    planeTexCoordBuffer->SetData(planeTexCoords, 12);

    auto f = make_shared<Object>();
    f->AttachBuffer(0, planeVertexBuffer);
    f->AttachBuffer(1, planeTexCoordBuffer, 2);

    for (int i = 0; i < CLOUDS; ++i)
    {
        Vector position(RandF(-15, 15), RandF(-15, 15), RandF(-15, 15));
        Vector size(RandF(2, 10), RandF(2, 10), RandF(2, 10));
        float colour = RandF(0.5f, 1);

        for (int j = 0; j < FLUFFS_PER_CLOUD; ++j)
        {
            Vector fluffPosition(RandF(-size.x / 2, size.x / 2), RandF(-size.y / 2, size.y / 2), RandF(-size.z / 2, size.z / 2));

            auto fluff = make_shared<RenderObject>();
            fluff->object = f;
            fluff->program = "fluff|fluff";
            fluff->transformation = Matrix::Translate(position) * Matrix::Translate(fluffPosition) * Matrix::Scale(RandF(2, 10)) * Matrix::Rotate(0, 0, RandF(0, PI * 2));
            fluff->colour[0] = colour;
            fluffs_.push_back(fluff);

            fluffNormals_.push_back(fluffPosition.Normalise());
        }
    }

    // Sort the renderObject and normal vectors based on the renderObjects' z translation. At least
    // c++11 makes this slightly less like being punched in the balls
    vector<pair<Vector, shared_ptr<RenderObject>>> ouch(fluffs_.size());
    int i = 0;
    for (int i = 0; i < fluffs_.size(); ++i)
        ouch[i] = make_pair(fluffNormals_[i], fluffs_[i]);

    // Sort our new FrankenCollection
    sort(ouch.begin(), ouch.end(), [](const pair<Vector, shared_ptr<RenderObject>> &a, const pair<Vector, shared_ptr<RenderObject>> &b) {
        return a.second->transformation.a[3][2] > b.second->transformation.a[3][2];
    });

    // Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn.
    for (int i = 0; i < ouch.size(); ++i)
    {
        fluffNormals_[i] = ouch[i].first;
        fluffs_[i] = ouch[i].second;
    }
    
    cloudTexture_ = LoadTexture("clouds.png");

    flashTime_ = 0;
    lightRotation_ = 0;

    renderer->SetCameraPosition(Vector(0, 0, 20));
    renderer->SetCameraLookAt(Vector(0, 0, -1));
}

Vector cameraPos(0, 0, 20);

void CloudNineWorld::Upate(float msecs)
{
    if (flashTime_ < 0 && RandF(0, 1) > 0.98f)
    {
        float bounds = 15;
        flashTime_ = LIGHTNING_TIME;
        flashPosition_ = Vector(RandF(-bounds, bounds), RandF(-bounds, bounds), RandF(-bounds, bounds));
    }
    flashTime_ -= msecs;

    lightRotation_ += 0.0002f * msecs;
    if (lightRotation_ > PI * 2)
        lightRotation_ -= PI * 2;

    cameraPos.z -= 0.0015f * msecs;
}

void CloudNineWorld::Draw(shared_ptr<Renderer> renderer)
{
    renderer->SetCameraPosition(cameraPos);
    renderer->SetCameraLookAt(cameraPos + Vector(0, 0, -1));

    glDisable(GL_CULL_FACE);

    renderer->SetUniform("fluff|fluff", 0, Vector(sinf(lightRotation_), -2, cosf(lightRotation_))); // lightDir
    renderer->SetUniform("fluff|fluff", 1, flashTime_ < 0 ? 0 : powf(flashTime_ / LIGHTNING_TIME, 2)); // lightStrength
    renderer->SetUniform("fluff|fluff", 2, flashPosition_); // lightPosition

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cloudTexture_);
    renderer->SetUniform("fluff|fluff", 4, 0);

    int fluffIndex = 0;
    for (auto fluff : fluffs_)
    {
        renderer->SetUniform("fluff|fluff", 3, fluffNormals_[fluffIndex]);
        renderer->Draw(fluff);
        fluffIndex ++;
    }
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<CloudNineWorld>();

    world->Init(renderer);
    return w.Loop(world, renderer);
}
