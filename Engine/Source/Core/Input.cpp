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

    glm::vec3 Input::GetMovementDirection(glm::vec2 input)
    {
        glm::vec3 forward = s_camera->GetForward();
        glm::vec3 right = s_camera->GetRight();

        forward.y = 0.0f;
        right.y = 0.0f;

        return glm::normalize(
            forward * input.y +
            right * input.x
        );
    }
}