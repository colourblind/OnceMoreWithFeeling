#include "Utils.h"
#include <algorithm>

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

Spline::Spline(std::vector<Vector> &points) : points_(points)
{
    int numPoints = points_.size();
    points_.push_back(points_[numPoints - 1]);
    points_.insert(points_.begin(), points_[0]);
}

Vector Spline::Get(float f)
{
    int maxIndex = points_.size() - 1;
    int scale = points_.size() - 2;
    float norm = (f * scale) + 1;
    int index = static_cast<int>(floor(norm));
    float offset = norm - index;

    int i0 = index - 1;
    int i1 = index;
    int i2 = min(maxIndex, index + 1);
    int i3 = min(maxIndex, index + 2);

    Vector p0 = points_[i0];
    Vector p1 = points_[i1];
    Vector p2 = points_[i2];
    Vector p3 = points_[i3];

    return (
        (p1 * 2) +
        (p0 * -1 + p2) * offset +
        (p0 * 2 - p1 * 5 + p2 * 4 - p3) * offset * offset +
        (p0 * -1 + p1 * 3 - p2 * 3 + p3) * offset * offset * offset
    ) * 0.5f;
}

vector<Vector> Spline::GetPoints(unsigned int count)
{
    vector<Vector> result;
    float step = 1.f / (count - 1);
    float f = 0;
    for (unsigned int i = 0; i < count; i++, f += step)
        result.push_back(Get(f));
    return result;
}
