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
const float ROTATION_SPEED = 0.0025f;
const float SPAWN_CHANCE = 0.1f;        // Smaller makes a spawn more likely on each tick

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
            hexes_.push_back(hexObject);

            rotations_.push_back(-1);
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
            float r = rotations_[i];
            if (r >= 0)
            {
                r = r + ROTATION_SPEED * msecs;
                if (r > PI)
                    r = -1;
                else if (rotations_[i] < PI / 4 && r > PI / 4)
                {
                    int numBranches = RandI(0, 2);
                    for (int n = 0; n < numBranches; ++ n)
                    {
                        // Select attached hex and start that one moving, if available
                        int selectionOffset = RandI(0, 5);
                        for (int j = 0; j < 6; ++j)
                        {
                            int x0, y0;
                            switch ((selectionOffset + j) % 6)
                            {
                                case 0:
                                    x0 = -1;
                                    y0 = -1;
                                    break;
                                case 1:
                                    x0 = 0;
                                    y0 = -1;
                                    break;
                                case 2:
                                    x0 = -1;
                                    y0 = 1;
                                    break;
                                case 3:
                                    x0 = 0;
                                    y0 = 1;
                                    break;
                                case 4:
                                    x0 = 0;
                                    y0 = -2;
                                    break;
                                case 5:
                                    x0 = 0;
                                    y0 = 2;
                                    break;
                            }
                            int f = (y + y0) * COUNT_X + (x + x0);
                            if (f < 0 || f >= COUNT_X * COUNT_Y)
                                continue;
                            if (rotations_[f] < 0)
                            {
                                rotations_[f] = 0;
                                break;
                            }
                        }
                    }
                }
            }

            if (r >= 0)
            {
                Vector pos = Matrix::Scale(0.1f) *
                    Matrix::Translate(Vector(x * (3.f + PADDING * 2) + offset, y * (0.866f + PADDING), 0)) *
                    Matrix::Translate(Vector((-COUNT_X / 2) * (3.f + PADDING * 2), (-COUNT_Y / 2) * (0.866f + PADDING), 0)) *
                    Vector(0, 0, 0);

                hexes_[i]->transformation = Matrix::Scale(0.1f) *
                    Matrix::Translate(Vector(x * (3.f + PADDING * 2) + offset, y * (0.866f + PADDING), 0)) *
                    Matrix::Translate(Vector((-COUNT_X / 2) * (3.f + PADDING * 2), (-COUNT_Y / 2) * (0.866f + PADDING), 0)) *
                    Matrix::Billboard(Vector(0, 0, 7), pos) *
                    Matrix::Rotate(0, r - PI / 2, 0);
            }
            rotations_[i] = r;
        }
    }

    if (RandF(0, SPAWN_CHANCE) < msecs / 1000)
    {
        int c = 0;
        int x = static_cast<int>(RandF(0, static_cast<float>(COUNT_X)));
        int y = static_cast<int>(RandF(0, static_cast<float>(COUNT_Y)));
        int i = y * COUNT_X + x;
        while (c < 20 && rotations_[i] >= 0)
        {
            x = static_cast<int>(RandF(0, static_cast<float>(COUNT_X)));
            y = static_cast<int>(RandF(0, static_cast<float>(COUNT_Y)));
            i = y * COUNT_X + x;
            c ++;
        }
        if (c < 20)
        {
            rotations_[i] = 0;
        }
    }
}

void HexFieldWorld::Draw(shared_ptr<Renderer> renderer)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Input input;
    if (input.GetState(0x4C)) // L key
        return; 
    if (input.PushedThisTick(0x4B)) // K
    {
        for (auto hex : hexes_)
            hex->colour[2] = 1 - hex->colour[2];
    }

    int i = 0;
    for (auto hex : hexes_)
    {
        // This deliberately ignores 0, so we don't draw a hex without it's
        // transformation initialised
        if (rotations_[i] > 0) 
        {
            renderer->SetUniform(hex->program, 0, rotations_[i]);
            renderer->Draw(hex);
        }
        i ++;
    }
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
