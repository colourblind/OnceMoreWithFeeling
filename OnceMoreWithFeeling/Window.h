#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Lib/glew.h"
#include <memory>

#include "World.h"
#include "Renderer.h"
#include "Input.h"

namespace OnceMoreWithFeeling
{
    class Window
    {
    public:
        Window(HINSTANCE instance, int showWindow, unsigned int width = 640, unsigned int height = 480);
        ~Window();

        int Loop(std::shared_ptr<World> world, std::shared_ptr<Renderer> renderer);
        static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

    protected:
        static std::shared_ptr<World> world_;
        static std::shared_ptr<Renderer> renderer_;
        static Input input_;
        HWND window_;
        HDC deviceContext_;
        HGLRC renderContext_;
        unsigned int width_;
        unsigned int height_;
    };
}
