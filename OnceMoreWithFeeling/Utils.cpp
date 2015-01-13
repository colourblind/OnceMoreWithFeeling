#include "Utils.h"
#include <algorithm>

using namespace OnceMoreWithFeeling;
using namespace std;

// We're avoiding the usual rand functions since we need coherent noise
// and don't want to mess around with seeds and potentially break other
// parts of the code
float Noise(int x, int y)
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (0.5f * static_cast<float>((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.f);
}

// Coherent noise with cosine interpolation
float SmoothNoise(float x, float y)
{
    int fx = static_cast<int>(floor(x));
    int fy = static_cast<int>(floor(y));

    float v = Noise(fx, fy);
    float vx = Noise(fx + 1, fy);
    float vy = Noise(fx, fy + 1);
    float vxy = Noise(fx + 1, fy + 1);

    float tx = x - fx;
    float ty = y - fy;
    float i0 = Coserp(v, vx, tx);
    float i1 = Coserp(vy, vxy, tx);
    return Coserp(i0, i1, ty);
}

// Fractal Brownian Motion
void OnceMoreWithFeeling::CreateNoise(vector<float> &data, int octaves, int width, int height, float frequencyScale)
{
    data.resize(width * height, 0);
    
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float frequency = frequencyScale / max(width, height);
            float amplitude = 0.5f;
            float result = 0;
            for (int o = 0; o < octaves; ++o)
            {
                result += SmoothNoise(static_cast<float>(x) * frequency, static_cast<float>(y) * frequency) * amplitude;
                frequency *= 2;
                amplitude *= 0.5f;
            }
            data[y * width + x] = result;
        }
    }
}

float OnceMoreWithFeeling::Lerp(float a, float b, float t)
{
    return a * (1.f - t) + b * t;
}

float OnceMoreWithFeeling::Coserp(float a, float b, float t)
{
    float c = (1.f + cos(t * PI)) * 0.5f;
    return a * c + b * (1.f - c);
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
