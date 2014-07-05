#pragma once

#include <Windows.h>
#include "glew.h"
#include <memory>

#include "Renderer.h"

namespace OnceMoreWithFeeling
{

    class Window
    {
    public:
        Window(HINSTANCE instance, int showWindow);
        ~Window();

        int Loop(std::shared_ptr<Renderer> renderer);
        static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

    protected:
        HWND window_;
        HDC deviceContext_;
        HGLRC renderContext_;
        unsigned int width_;
        unsigned int height_;
    };
}
