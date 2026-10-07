#include "Core/Input.h"

namespace MiraEngine
{
    Window* Input::s_window = nullptr;
    bool Input::s_gameplayEnabled = true;

    void Input::Initialize(Window& window)

    {
        s_window = &window;
    }

    bool Input::IsKeyPressed(
        Key key
    )
    {
        if (
            !s_window ||
            !s_gameplayEnabled
            )
        {
            return false;
        }

        return s_window->IsKeyPressed(
            key
        );
    }

    void Input::SetGameplayEnabled(
        bool enabled
    )
    {
        s_gameplayEnabled = enabled;
    }

    bool Input::IsGameplayEnabled()
    {
        return s_gameplayEnabled;
    }
}