#include "Window.h"
#include "Renderer.h"
#include "World.h"
#include "Utils.h"

using namespace OnceMoreWithFeeling;
using namespace std;

class HexFieldWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    vector<shared_ptr<RenderObject>> hexes_;
    vector<float> noise_;
    vector<float> rotations_;
};

const float PADDING = 0.1f;
const int COUNT_Y = 64;
const int COUNT_X = 32;
const float ROTATION_SPEED = 0.001f;

float hexPoints[] = {
    1, 0, 0,
    0.5f, 0.866f, 0,
    0.5f, -0.866f, 0,
    
    -1, 0, 0,
    -0.5f, 0.866f, 0,
    -0.5f, -0.866f, 0,

    0.5f, 0.866f, 0,
    0.5f, -0.866f, 0,
    -0.5f, 0.866f, 0,

    -0.5f, 0.866f, 0,
    -0.5f, -0.866f, 0,
    0.5f, -0.866f, 0
};

void HexFieldWorld::Init(shared_ptr<Renderer> renderer)
{
    renderer->AddShader("basic", "basic");

    auto hexBuffer = make_shared<Buffer>();
    hexBuffer->SetData(hexPoints, 36);
    auto hexArray = make_shared<Object>();
    hexArray->AttachBuffer(0, hexBuffer);

    CreateNoise(noise_, 8, COUNT_X, COUNT_Y);

    for (int y = 0; y < COUNT_Y; ++y)
    {
        float offset = y % 2 == 0 ? 0 : (1.5f + PADDING);
        for (int x = 0; x < COUNT_X; ++x)
        {
            float noiseValue = noise_[y * COUNT_X + x];

            auto hexObject = make_shared<RenderObject>();
            hexObject->object = hexArray;
            hexObject->program = "basic|basic";
            hexObject->colour[0] = 1 * noiseValue;
            hexObject->colour[1] = 0.9f * noiseValue;
            hexObject->colour[2] = 0.1f * noiseValue;
            hexObject->transformation = Matrix::Scale(0.1f) * Matrix::Translate(Vector(x * (3.f + PADDING * 2) + offset, y * (0.866f + PADDING), 0)) * Matrix::Translate(Vector((-COUNT_X / 2) * (3.f + PADDING * 2), (-COUNT_Y / 2) * (0.866f + PADDING), 0));
            hexes_.push_back(hexObject);

            rotations_.push_back(RandF(0, 2 * PI));
        }
    }
}

void HexFieldWorld::Upate(float msecs)
{
    for (int y = 0; y < COUNT_Y; ++y)
    {
        float offset = y % 2 == 0 ? 0 : (1.5f + PADDING);
        for (int x = 0; x < COUNT_X; ++x)
        {
            int i = y * COUNT_X + x;
            rotations_[i] += ROTATION_SPEED * msecs;
            if (rotations_[i] > 2 * PI)
                rotations_[i] -= 2 * PI;
            hexes_[i]->transformation = Matrix::Scale(0.1f) * Matrix::Translate(Vector(x * (3.f + PADDING * 2) + offset, y * (0.866f + PADDING), 0)) * Matrix::Translate(Vector((-COUNT_X / 2) * (3.f + PADDING * 2), (-COUNT_Y / 2) * (0.866f + PADDING), 0)) * Matrix::Rotate(0, rotations_[i], 0);
        }
    }
}

void HexFieldWorld::Draw(shared_ptr<Renderer> renderer)
{
    for (auto hex : hexes_)
        renderer->Draw(hex);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<HexFieldWorld>();

    world->Init(renderer);
    return w.Loop(world, renderer);
}
