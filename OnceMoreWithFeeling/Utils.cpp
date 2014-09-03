#include "Utils.h"
#include "Maths.h"

using namespace OnceMoreWithFeeling;
using namespace std;

void FillCube(vector<float> &data, float value, int x, int y, int z, int width, int height, int depth, int size)
{
    for (int i = x; i < x + size; ++i)
    {
        for (int j = y; j < y + size; ++j)
        {
            for (int k = z; k < z + size; ++k)
            {
                data[i * width * height + j * height + k] += value;
            }
        }
    }
}

void OnceMoreWithFeeling::CreateNoise(vector<float> &data, int octaves, int width, int height, int depth)
{
    data.resize(width * height * depth, 0);

    for (int o = 0; o < octaves; ++o)
    {
        int step = static_cast<int>(powf(2, static_cast<float>(o)));
        float max = 1.f / powf(2, static_cast<float>(octaves - o));
        for (int x = 0; x < width; x += step)
        {
            for (int y = 0; y < height; y += step)
            {
                for (int z = 0; z < depth; z += step)
                {
                    FillCube(data, RandF(0, max), x, y, z, width, height, depth, step);
                }
            }
        }
    }
}
