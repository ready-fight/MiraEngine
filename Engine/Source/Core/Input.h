#pragma once

#include "Core/Window.h"

namespace MiraEngine
{
    class Input
    {
    public:
        static void Initialize(Window& window);

        static bool IsKeyPressed(Key key);

    private:
        static Window* s_window;
    };
}