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
    };

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void StartFrame();
        void EndFrame();
        void Draw(std::shared_ptr<RenderObject> renderObject, GLenum type = GL_TRIANGLES);

        void SetCameraPosition(Vector cameraPosition) { cameraPosition_ = cameraPosition; }
        void SetCameraLookAt(Vector cameraLookAt) { cameraLookAt_ = cameraLookAt; }

        void AddShader(std::string vertexShaderName, std::string fragmentShaderName);
        void SetWindowSize(unsigned int width, unsigned int height);
        void ResetFrameCount() { fps_ = frameCount_; frameCount_ = 0; }

        void SetUniform(std::string program, int location, int value);
        void SetUniform(std::string program, int location, float value);
        void SetUniform(std::string program, int location, Vector value);

    protected:
        std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaders_;
        unsigned int width_;
        unsigned int height_;
        Font font_;
        unsigned int frameCount_;
        unsigned int fps_;
        Matrix projection_;
        Matrix view_;
        Vector cameraPosition_;
        Vector cameraLookAt_;
    };
}
