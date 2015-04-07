#pragma once

#include <cstdint>

namespace OnceMoreWithFeeling
{
    class Input
    {
    public:
        void Init();
        void SetDown(uint8_t key) { pushedThisTick_[key] = true; keyStates_[key] = true; }
        void SetUp(uint8_t key) { keyStates_[key] = false; }

        bool GetState(uint8_t key) const { return keyStates_[key]; }
        bool PushedThisTick(uint8_t key) const { return pushedThisTick_[key]; }
        void ResetTick();
        void ToggleMouse();
        void GetMouseOffset(float *x, float *y) { *x = mouseXOffset_; *y = mouseYOffset_; }

    private:
        static bool keyStates_[256];
        static bool pushedThisTick_[256];
        static float mouseXOffset_;
        static float mouseYOffset_;
        static bool mouseLocked_;
    };
}
