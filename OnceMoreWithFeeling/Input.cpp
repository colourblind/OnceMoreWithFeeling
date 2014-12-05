#include "Input.h"

using namespace OnceMoreWithFeeling;

bool Input::keyStates_[256];
bool Input::pushedThisTick_[256];

void Input::Init()
{
    for (int i = 0; i < 256; ++i)
    {
        keyStates_[i] = false;
        pushedThisTick_[i] = false;
    }
}

void Input::ResetTick()
{
    for (int i = 0; i < 256; ++i)
    {
        pushedThisTick_[i] = false;
    }
}
