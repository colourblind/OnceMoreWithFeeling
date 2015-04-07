#include "Input.h"
#include <Windows.h>

using namespace OnceMoreWithFeeling;

bool Input::keyStates_[256];
bool Input::pushedThisTick_[256];
float Input::mouseXOffset_ = 0;
float Input::mouseYOffset_ = 0;
bool Input::mouseLocked_ = false;

void Input::Init()
{
    for (int i = 0; i < 256; ++i)
    {
        keyStates_[i] = false;
        pushedThisTick_[i] = false;
    }
    mouseLocked_ = false;
}

void Input::ResetTick()
{
    if (pushedThisTick_[VK_PAUSE])
        ToggleMouse();

    for (int i = 0; i < 256; ++i)
    {
        pushedThisTick_[i] = false;
    }
    if (mouseLocked_)
    {
        POINT mousePos;
        ::GetCursorPos(&mousePos);
        mouseXOffset_ = static_cast<float>(mousePos.x - 200);
        mouseYOffset_ = static_cast<float>(mousePos.y - 200);
        ::SetCursorPos(200, 200);
    }
    else
    {
        mouseXOffset_ = 0;
        mouseYOffset_ = 0;
    }
}

void Input::ToggleMouse()
{
    mouseLocked_ = !mouseLocked_;
    ::ShowCursor(!mouseLocked_);
    if (mouseLocked_)
    {
        ::SetCursorPos(200, 200);
    }
}
