#include "Objects.h"

using namespace OnceMoreWithFeeling;
using namespace std;

Object::Object() : count_(0)
{
    glGenVertexArrays(1, &arrayObject_);
}

Object::~Object()
{
    glDeleteVertexArrays(1, &arrayObject_);
}

void Object::Draw(GLenum type)
{
    Activate();
    glDrawArrays(type, 0, count_);
    Deactivate();
}

void Object::AttachBuffer(int attribLocation, shared_ptr<Buffer> buffer, unsigned int size)
{
    Activate();
    buffer->Activate();
    glEnableVertexAttribArray(attribLocation);
    glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, 0, 0);
    buffer->Deactivate();
    Deactivate();
    count_ = buffer->GetSize() / size;
    // Not sure if this is strictly necessary . . .
    buffers_.push_back(buffer);
}

Buffer::Buffer() : count_(0)
{
    glGenBuffers(1, &bufferObject_);
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &bufferObject_);
}

void Buffer::SetData(float *data, unsigned int count)
{
    Activate();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, data, GL_STATIC_DRAW);
    Deactivate();
    count_ = count;
}

void Buffer::SetData(vector<float> &data)
{
    SetData(&data[0], data.size());
}
