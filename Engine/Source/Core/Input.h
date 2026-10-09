#pragma once

#include "Core/Window.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace MiraEngine
{
    class Camera;

    class Input
    {
    public:

        Input() = delete;

        static void Initialize(Window& window, Camera& camera);

        static bool IsKeyPressed(Key key);

        static void SetGameplayEnabled(
            bool enabled
        );
        static bool IsGameplayEnabled();

        static glm::vec3 GetMovementDirection(glm::vec2 input);

        static bool IsMouseButtonPressed(
            MouseButton button
        );

        static MouseMovement GetMouseMovement();

        static void CaptureCursor();
        static void ReleaseCursor();
        static float GetMouseScroll();

    private:
        static Window* s_window;
        static Camera* s_camera;
        static bool s_gameplayEnabled;
    };
}