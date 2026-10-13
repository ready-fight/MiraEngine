#include "Core/Input.h"
#include "Graphics/Camera.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace MiraEngine
{
    Window* Input::s_window = nullptr;
    Camera* Input::s_camera = nullptr;
    bool Input::s_gameplayEnabled = true;

    void Input::Initialize(Window& window, Camera& camera)

    {
        s_window = &window;
        s_camera = &camera;
    }

    void Input::SetGameplayEnabled(bool enabled)
    {
        s_gameplayEnabled = enabled;
    }

    bool Input::IsGameplayEnabled()
    {
        return s_gameplayEnabled;
    }

    // While the F1 editor camera is active, gameplay input reads as "nothing pressed".
    bool Input::IsKeyPressed(Key key)
    {
        return s_gameplayEnabled && s_window->IsKeyPressed(key);
    }

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        return s_gameplayEnabled && s_window->IsMouseButtonPressed(button);
    }

    MouseMovement Input::GetMouseMovement()
    {
        return s_gameplayEnabled ? s_window->GetMouseMovement() : MouseMovement{};
    }

    float Input::GetMouseScroll()
    {
        return s_gameplayEnabled ? s_window->GetMouseScroll() : 0.0f;
    }

    void Input::CaptureCursor()
    {
        s_window->CaptureCursor();
    }

    void Input::ReleaseCursor()
    {
        s_window->ReleaseCursor();
    }

    glm::vec3 Input::GetMovementDirection(glm::vec2 input)
    {
        glm::vec3 forward = s_camera->GetForward();
        glm::vec3 right = s_camera->GetRight();
        forward.y = 0.0f;
        right.y = 0.0f;

        return glm::normalize(forward * input.y + right * input.x);
    }
}