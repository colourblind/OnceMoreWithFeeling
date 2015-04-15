#pragma once

#include "Renderer.h"

namespace OnceMoreWithFeeling
{
    class World
    {
    public:
        virtual ~World() { }
        virtual void Init(std::shared_ptr<Renderer> renderer) { }
        virtual void Upate(float msecs) { }
        virtual void Draw(std::shared_ptr<Renderer> renderer) { }
        virtual void Resize(std::shared_ptr<Renderer> renderer) { }
    };
}
