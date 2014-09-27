#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

// Uncomment this to render at 25fps and write every frame for a PNG file.
// Also removes the FPS counter and is really slow.
// TODO: move it into a new build configuration
//#define OMWF_RECORDING

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
        std::unordered_map<unsigned int, std::string> textureBindings;
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
        void AddTexture(std::string textureName);
        void AddCubeTexture(std::string textureName, std::vector<std::string> filenames);
        void SetWindowSize(unsigned int width, unsigned int height);
        void ResetFrameCount() { fps_ = frameCount_; frameCount_ = 0; }

        void SetUniform(std::string program, int location, int value);
        void SetUniform(std::string program, int location, float value);
        void SetUniform(std::string program, int location, Vector value);

        void SetTextures(std::string program, std::unordered_map<unsigned int, std::string> binding);

    protected:
        std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> shaders_;
        std::unordered_map<std::string, std::pair<GLenum, GLuint>> textures_;
        unsigned int width_;
        unsigned int height_;
        Font font_;
        unsigned int frameCount_;
        unsigned int totalFrameCount_;
        unsigned int fps_;
        Matrix projection_;
        Matrix view_;
        Vector cameraPosition_;
        Vector cameraLookAt_;
    };
}
