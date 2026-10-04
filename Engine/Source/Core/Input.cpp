#include "Core/Input.h"

namespace MiraEngine
{
    Window* Input::s_window = nullptr;

    void Input::Initialize(Window& window)
    {
        s_window = &window;
    }

    bool Input::IsKeyPressed(Key key)
    {
        if (!s_window)
        {
            return false;
        }

        return s_window->IsKeyPressed(key);
    }
}