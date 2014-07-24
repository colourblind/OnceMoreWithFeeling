#include "Window.h"
#include "wglew.h"
#include <memory>

using namespace OnceMoreWithFeeling;
using namespace std;

shared_ptr<Renderer> Window::renderer_ = nullptr;

LRESULT CALLBACK Window::WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC deviceContext;

    switch (message)
    {
        case WM_PAINT:
            deviceContext = ::BeginPaint(window, &ps);
            ::EndPaint(window, &ps);
            break;
        case WM_SIZE:
            renderer_->SetWindowSize(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        default:
            return ::DefWindowProc(window, message, wParam, lParam);
    }
    
    return 0;
}

Window::Window(HINSTANCE instance, int show) : width_(640), height_(480)
{
    WNDCLASSEX windowClass;
    windowClass.cbClsExtra      = 0;
    windowClass.cbSize          = sizeof(WNDCLASSEX);
    windowClass.cbWndExtra      = 0;
    windowClass.hbrBackground   = nullptr;
    windowClass.hCursor         = nullptr;
    windowClass.hIcon           = nullptr;
    windowClass.hIconSm         = nullptr;
    windowClass.hInstance       = instance;
    windowClass.lpfnWndProc     = WndProc;
    windowClass.lpszClassName   = "OnceMoreWithFeeling";
    windowClass.lpszMenuName    = nullptr;
    windowClass.style           = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    if (!RegisterClassEx(&windowClass))
    {
        // Crash and burn
    }

    window_ = ::CreateWindow(
        "OnceMoreWithFeeling",
        "OnceMoreWithFeeling",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT, 
        width_,
        height_,
        nullptr,
        nullptr,
        instance,
        nullptr
    );

    deviceContext_ = ::GetDC(window_);

    PIXELFORMATDESCRIPTOR pixelFormat;
    memset(&pixelFormat, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pixelFormat.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

    int pf = ::ChoosePixelFormat(deviceContext_, &pixelFormat);
    if (!::SetPixelFormat(deviceContext_, pf, &pixelFormat))
    {
        // Crash and burn
    }

    ::ShowWindow(window_, show);
    ::UpdateWindow(window_);

    auto tempGlContext = ::wglCreateContext(deviceContext_);
    ::wglMakeCurrent(deviceContext_, tempGlContext);

    if (glewInit() != GLEW_OK)
    {
        // Crash and burn
    }

    if (wglewIsSupported("WGL_ARB_create_context") == 1)
    {
        int attributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 0,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        
        renderContext_ = ::wglCreateContextAttribsARB(deviceContext_, nullptr, attributes);
        ::wglMakeCurrent(nullptr, nullptr);
        ::wglDeleteContext(tempGlContext);
        ::wglMakeCurrent(deviceContext_, renderContext_);
    }
    else 
    {
        // Crash and burn, you luddite
    }
}

Window::~Window()
{
    ::wglMakeCurrent(deviceContext_, 0);
    ::wglDeleteContext(renderContext_);
    ::ReleaseDC(window_, deviceContext_);
}

int Window::Loop(shared_ptr<World> world, shared_ptr<Renderer> renderer)
{
    renderer_ = renderer;
    renderer_->SetWindowSize(width_, height_);

    LARGE_INTEGER freq, counter, last;
    ::QueryPerformanceFrequency(&freq);
    ::QueryPerformanceCounter(&last);
    float toMsecs = 1000.f / freq.QuadPart;
    float msecCounter = 0;

    // Main message loop:
    MSG msg;
    while (true)
    {
        if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
				break;
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            ::QueryPerformanceCounter(&counter);
            float msecs = (counter.QuadPart - last.QuadPart) * toMsecs;
            last = counter;
            world->Upate(msecs);

            renderer_->StartFrame();
            world->Draw(renderer);
            renderer_->EndFrame();

            ::SwapBuffers(deviceContext_);

            msecCounter += msecs;
            if (msecCounter > 1000)
            {
                renderer_->ResetFrameCount();
                msecCounter -= 1000;
            }
        }
    }

    return static_cast<int>(msg.wParam);
}
