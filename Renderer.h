#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "ShaderProgram.h"
#include "Objects.h"

namespace OnceMoreWithFeeling
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void Render();
        
        void AddShader(std::string vertexShaderName, std::string fragmentShaderName);
        void AddObject(std::shared_ptr<Object> object) { objects_.push_back(object); }

    protected:
        std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaders_;
        std::vector<std::shared_ptr<Object>> objects_;
    };
}
