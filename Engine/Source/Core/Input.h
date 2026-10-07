#pragma once

#include "Core/Window.h"

namespace MiraEngine
{
    class Input
    {
    public:
        static void Initialize(Window& window);

        static bool IsKeyPressed(Key key);

        static void SetGameplayEnabled(
            bool enabled
        );

        static bool IsGameplayEnabled();

    private:
        static Window* s_window;
        static bool s_gameplayEnabled;
    };
}