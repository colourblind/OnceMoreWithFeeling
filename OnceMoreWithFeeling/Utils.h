#pragma once

#include <vector>
#include "Maths.h"

namespace OnceMoreWithFeeling
{
    void CreateNoise(std::vector<float> &data, int octaves, int width, int height, float frequencyScale = 1.f);
    void CreateShitNoise(std::vector<float> &data, int width, int height);
    float Lerp(float a, float b, float t);
    float Coserp(float a, float b, float t);

    class Spline
    {
    public:
        Spline(std::vector<Vector> &points);
        virtual Vector Get(float f);
        std::vector<Vector> GetPoints(unsigned int count);

    protected:
        std::vector<Vector> points_;
    };
}
