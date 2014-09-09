#pragma once

#include <vector>
#include "Maths.h"

namespace OnceMoreWithFeeling
{
    void CreateNoise(std::vector<float> &data, int octaves, int width, int height, int depth);

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
