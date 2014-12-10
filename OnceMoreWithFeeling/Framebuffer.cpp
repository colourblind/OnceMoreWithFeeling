#include "Framebuffer.h"

using namespace OnceMoreWithFeeling;

Framebuffer::Framebuffer(unsigned int width, unsigned int height) :
    width_(width),
    height_(height)
{
    // Colour attachment
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Depth renderbuffer
    glGenRenderbuffers(1, &depth_);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width_, height_);

    // Framebuffer and attachments
    glGenFramebuffers(1, &handle_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle_);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_, 0);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &handle_);
    glDeleteTextures(1, &texture_);
    glDeleteRenderbuffers(1, &depth_);
}

void Framebuffer::Activate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, handle_);
}

void Framebuffer::Deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
