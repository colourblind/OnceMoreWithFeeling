#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "ShaderProgram.h"
#include "Objects.h"
#include "Fonts.h"
#include "Maths.h"

namespace OnceMoreWithFeeling
{
    struct RenderObject
    {
        std::shared_ptr<Object> object;
        std::string program;
        Matrix transformation;
        float colour[3];
        float shininess;
    };

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void StartFrame();
        void EndFrame();
        void Render(float msecs);
        void Draw(std::shared_ptr<RenderObject> renderObject);
        
        void AddShader(std::string vertexShaderName, std::string fragmentShaderName);
        void AddObject(std::shared_ptr<RenderObject> object) { objects_.push_back(object); }
        void SetWindowSize(unsigned int width, unsigned int height);
        void ResetFrameCount() { fps_ = frameCount_; frameCount_ = 0; }

    protected:
        std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaders_;
        std::vector<std::shared_ptr<RenderObject>> objects_;
        unsigned int width_;
        unsigned int height_;
        Font font_;
        unsigned int frameCount_;
        unsigned int fps_;
        Matrix projection_;
        Matrix view_;
    };
}
