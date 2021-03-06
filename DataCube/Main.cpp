#include <Windows.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Window.h"
#include "Renderer.h"
#include "World.h"

using namespace OnceMoreWithFeeling;
using namespace std;

enum class RenderMode { CUBE, SPHERE, TERRAIN };

class DataCubeWorld : public World
{
public:
    virtual void Init(shared_ptr<Renderer> renderer);
    virtual void Upate(float msecs);
    virtual void Draw(shared_ptr<Renderer> renderer);

private:
    void LoadData(wstring filename, int dimensions, vector<float> &result, unsigned long *activePoints, float *minValue, float *maxValue);
    void CreateCube(shared_ptr<Buffer> vertexBuffer, shared_ptr<Buffer> colourBuffer);
    Vector GetColour(float f, float mean);

    shared_ptr<RenderObject> object_;
    float rotation_;
    RenderMode renderMode_;
    wstring targetFile_;
    bool animating_;
};

void DataCubeWorld::LoadData(wstring filename, int dimensions, vector<float> &result, unsigned long *activePoints, float *minValue, float *maxValue)
{
    ifstream f;
    f.open(filename, ios::in | ios::binary);

    f.seekg(0, ios::end);
    streampos length = f.tellg();
    f.seekg(0, ios::beg);

    result = vector<float>(static_cast<int>(pow(256, static_cast<int>(dimensions))));

    float s = 1.f / length;
    *activePoints = 0;

    vector<unsigned char> buffer(dimensions * 1024);
    size_t bufferSize = buffer.size();
    unsigned int bufferIndex = bufferSize;

    while (!f.eof())
    {
        if (bufferIndex >= bufferSize)
        {
            f.read(reinterpret_cast<char *>(&(buffer[0])), bufferSize);
            bufferIndex = 0;
        }

        unsigned int index = 0;
        for (int i = 0; i < dimensions; ++i)
        {
            unsigned char c = buffer[bufferIndex + i];
            index += static_cast<unsigned int>(c * pow(256, static_cast<int>(dimensions - 1 - i)));
        }
        bufferIndex += dimensions;

        if (result[index] < s)
            (*activePoints)++;
        result[index] += s;
    }
    f.close();

    *maxValue = 0;
    *minValue = 1;
    for (auto v : result)
    {
        if (v > 0)
        {
            *maxValue = max(*maxValue, v);
            *minValue = min(*minValue, v);
        }
    }
}

void DataCubeWorld::CreateCube(shared_ptr<Buffer> vertexBuffer, shared_ptr<Buffer> colourBuffer)
{
    vector<float> counts;
    unsigned long activePoints;
    float minValue, maxValue;
    switch (renderMode_)
    {
        case RenderMode::CUBE:
        case RenderMode::SPHERE:
            LoadData(targetFile_, 3, counts, &activePoints, &minValue, &maxValue);
            break;
        case RenderMode::TERRAIN:
            LoadData(targetFile_, 2, counts, &activePoints, &minValue, &maxValue);
            break;
    }

    // This is designed to map the mean to 0.5 when raised to the power s
    // in an attempt to normalise the results.
    float average = 0;
    for (auto v : counts)
    {
        if (v > 0)
            average += (v - minValue) / activePoints;
    }

    float scale = ::logf(0.5f) / ::logf(average);

    vector<float> vertexData;
    vector<float> colourData;
    for (int x = 0; x < 256; ++x)
    {
        for (int y = 0; y < 256; ++y)
        {
            if (renderMode_ == RenderMode::TERRAIN)
            {
                unsigned int index = x * 256 + y;
                vertexData.push_back(static_cast<float>(x));
                vertexData.push_back(powf(counts[index] / maxValue, scale));
                vertexData.push_back(static_cast<float>(y));

                Vector c = GetColour((counts[index] - minValue) / (maxValue - minValue), scale);
                colourData.push_back(c.x);
                colourData.push_back(c.y);
                colourData.push_back(c.z);
            }
            else
            {
                for (int z = 0; z < 256; ++z)
                {
                    unsigned int index = x * 256 * 256 + y * 256 + z;
                    if (counts[index] > 0)
                    {
                        if (renderMode_ == RenderMode::CUBE)
                        {
                            vertexData.push_back(static_cast<float>(x));
                            vertexData.push_back(static_cast<float>(y));
                            vertexData.push_back(static_cast<float>(z));
                        }
                        else if (renderMode_ == RenderMode::SPHERE)
                        {
                            float theta = (static_cast<float>(x) / 256) * PI * 2;
                            float phi = (static_cast<float>(y) / 256) * PI * 2;
                            float rho = static_cast<float>(z) / 255;
                            vertexData.push_back(rho * sinf(theta) * cosf(phi));
                            vertexData.push_back(rho * sinf(theta) * sinf(phi));
                            vertexData.push_back(rho * cosf(theta));
                        }

                        Vector c = GetColour((counts[index] - minValue) / (maxValue - minValue), scale);
                        colourData.push_back(c.x);
                        colourData.push_back(c.y);
                        colourData.push_back(c.z);
                    }
                }
            }
        }
    }

    vertexBuffer->SetData(vertexData);
    colourBuffer->SetData(colourData);
}

Vector DataCubeWorld::GetColour(float f, float scale)
{
    vector<Vector> c = {
        Vector(0, 0, 0.5f),
        Vector(0.75f, 0, 0.75f),
        Vector(0.8f, 0, 0),
        Vector(1, 1, 0),
        Vector(1, 1, 1)
    };
    f = pow(f, scale);
    f *= (c.size() - 1);
    
    Vector c0 = c[static_cast<int>(floor(f))];
    Vector c1 = c[static_cast<int>(ceil(f))];

    f = f - floor(f);

    return Vector(
        c0.x * (1 - f) + c1.x * f,
        c0.y * (1 - f) + c1.y * f,
        c0.z * (1 - f) + c1.z * f
    );
}

void DataCubeWorld::Init(shared_ptr<Renderer> renderer)
{
    int numArgs;
    LPWSTR *args = ::CommandLineToArgvW(::GetCommandLineW(), &numArgs);

    wstring r(args[1]);
    if (r.compare(L"cube") == 0)
        renderMode_ = RenderMode::CUBE;
    else if (r.compare(L"sphere") == 0)
        renderMode_ = RenderMode::SPHERE;
    else if (r.compare(L"terrain") == 0)
        renderMode_ = RenderMode::TERRAIN;

    targetFile_ = wstring(args[2]);

    renderer->AddShader("basic", "basic");

    if (renderMode_ == RenderMode::TERRAIN)
    {
        renderer->SetCameraPosition(Vector(0, 5, 6));
        renderer->SetCameraLookAt(Vector(0, 1.5, 0));
    }

    shared_ptr<Buffer> vertexBuffer = make_shared<Buffer>();
    shared_ptr<Buffer> colourBuffer = make_shared<Buffer>();
    CreateCube(vertexBuffer, colourBuffer);

    shared_ptr<Object> o = make_shared<Object>();
    o->AttachBuffer(0, vertexBuffer);
    o->AttachBuffer(1, colourBuffer);

    object_ = make_shared<RenderObject>();
    object_->object = o;
    object_->program = "basic|basic";

    rotation_ = 0;
    animating_ = true;
}

void DataCubeWorld::Upate(float msecs)
{
    Input input;
    if (input.PushedThisTick(VK_SPACE))
        animating_ = !animating_;

    if (animating_)
    {
        rotation_ += 0.001f * msecs;
        if (rotation_ > PI * 2)
            rotation_ -= PI * 2;
    }

    if (renderMode_ == RenderMode::CUBE)
        object_->transformation = Matrix::Scale(4.5f / 256) * Matrix::Rotate(0, rotation_, 0) * Matrix::Translate(-128, -128, -128);
    else if (renderMode_ == RenderMode::SPHERE)
        object_->transformation = Matrix::Scale(3) * Matrix::Rotate(0, rotation_, 0);
    else if (renderMode_ == RenderMode::TERRAIN)
        object_->transformation = Matrix::Scale(4.f / 256, 3, 4.f / 256) * Matrix::Rotate(0, rotation_, 0) * Matrix::Translate(-128, 0, -128);
}

void DataCubeWorld::Draw(shared_ptr<Renderer> renderer)
{
    glEnable(GL_DEPTH_BUFFER);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->Draw(object_, GL_POINTS);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR commandLine, int show)
{
    srand(::GetTickCount());

    OnceMoreWithFeeling::Window w(instance, show);
    shared_ptr<Renderer> renderer = make_shared<Renderer>();
    shared_ptr<World> world = make_shared<DataCubeWorld>();

    return w.Loop(world, renderer);
}
