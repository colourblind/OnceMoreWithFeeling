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

void Object::Draw()
{
    Activate();
    glDrawArrays(GL_TRIANGLES, 0, count_);
    Deactivate();
}

void Object::AttachBuffer(int attribLocation, shared_ptr<Buffer> buffer)
{
    Activate();
    buffer->Activate();
    glEnableVertexAttribArray(attribLocation);
    glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    buffer->Deactivate();
    Deactivate();
    count_ = buffer->GetSize();
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count * 3, data, GL_STATIC_DRAW);
    Deactivate();
    count_ = count;
}
