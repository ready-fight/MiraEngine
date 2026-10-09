#include "Camera/CameraController.h"

#include "Player/Player.h"

#include "Graphics/Camera.h"
#include "Core/Input.h"

#include <glm/common.hpp>
#include <glm/trigonometric.hpp>

#include <cmath>

namespace MiraGame
{
    CameraController::CameraController(
        MiraEngine::Camera& camera,
        Player& player
    )
        : MiraEngine::GameObject(
            "CameraController"
        ),
        m_camera(&camera),
        m_player(&player)
    {
    }

    void CameraController::Update(
        float deltaTime
    )
    {
        if (
            !m_camera ||
            !m_player
            )
        {
            return;
        }

        // F1 editor-camera mode is active.
        if (
            !MiraEngine::Input::
            IsGameplayEnabled()
            )
        {
            // Do not release the cursor here.
            // Application owns it in editor mode.
            m_orbiting = false;

            return;
        }

        const float scroll =
            MiraEngine::Input::
            GetMouseScroll();

        if (scroll != 0.0f)
        {
            m_distance -=
                scroll *
                m_zoomSpeed;

            m_distance =
                glm::clamp(
                    m_distance,
                    m_minDistance,
                    m_maxDistance
                );
        }

        const bool rightMouseDown =
            MiraEngine::Input::
            IsMouseButtonPressed(
                MiraEngine::
                MouseButton::Right
            );

        if (rightMouseDown)
        {
            if (!m_orbiting)
            {
                m_orbiting = true;

                MiraEngine::Input::
                    CaptureCursor();
            }

            const MiraEngine::MouseMovement
                mouse =
                MiraEngine::Input::
                GetMouseMovement();

            m_yaw +=
                static_cast<float>(
                    mouse.x
                    ) *
                m_mouseSensitivity;

            m_pitch -=
                static_cast<float>(
                    mouse.y
                    ) *
                m_mouseSensitivity;

            m_pitch =
                glm::clamp(
                    m_pitch,
                    -10.0f,
                    60.0f
                );
        }
        else if (m_orbiting)
        {
            m_orbiting = false;

            MiraEngine::Input::
                ReleaseCursor();
        }

        const glm::vec3 playerPosition =
            m_player
            ->GetTransform()
            .GetPosition();

        const glm::vec3 lookTarget =
            playerPosition +
            m_lookOffset;

        const float yaw =
            glm::radians(
                m_yaw
            );

        const float pitch =
            glm::radians(
                m_pitch
            );

        glm::vec3 cameraOffset;

        cameraOffset.x =
            std::cos(pitch) *
            std::cos(yaw) *
            m_distance;

        cameraOffset.y =
            std::sin(pitch) *
            m_distance;

        cameraOffset.z =
            std::cos(pitch) *
            std::sin(yaw) *
            m_distance;

        m_camera->SetPosition(
            lookTarget +
            cameraOffset
        );

        m_camera->LookAt(
            lookTarget
        );
    }
}