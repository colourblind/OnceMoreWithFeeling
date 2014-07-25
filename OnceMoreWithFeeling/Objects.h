#pragma once

#include <vector>
#include <memory>
#include "glew.h"

#include "ShaderProgram.h"

namespace OnceMoreWithFeeling
{
    class Buffer
    {
    public:
        Buffer();
        ~Buffer();

        void SetData(float *data, unsigned int count);
        void SetData(std::vector<float> &data);
        unsigned int GetSize() { return count_; }

        void Activate() { glBindBuffer(GL_ARRAY_BUFFER, bufferObject_); }
        static void Deactivate() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

    protected:
        GLuint bufferObject_;
        unsigned int count_;
    };
    
    class Object
    {
    public:
        Object();
        ~Object();

        void Draw();

        void Activate() { glBindVertexArray(arrayObject_); }
        static void Deactivate() { glBindVertexArray(0); }

        void AttachBuffer(int attribLocation, std::shared_ptr<Buffer> buffer, unsigned int size = 3);

    protected:
        std::vector<std::shared_ptr<Buffer>> buffers_;
        GLuint arrayObject_;

        unsigned int count_;
    };
}
