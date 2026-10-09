#pragma once

#include "Scene/GameObject.h"

#include <glm/vec3.hpp>

namespace MiraEngine
{
    class Camera;
}

namespace MiraGame
{
    class Player;

    class CameraController
        : public MiraEngine::GameObject
    {
    public:
        CameraController(
            MiraEngine::Camera& camera,
            Player& player
        );

        void Update(
            float deltaTime
        ) override;

    private:
        MiraEngine::Camera* m_camera = nullptr;
        Player* m_player = nullptr;

        float m_yaw = -90.0f;
        float m_pitch = 20.0f;
        float m_distance = 5.85f;
        float m_zoomSpeed = 0.75f;
        float m_minDistance = 2.5f;
        float m_maxDistance = 10.0f;

        float m_mouseSensitivity =
            0.15f;

        glm::vec3 m_lookOffset =
            glm::vec3(
                0.0f,
                1.0f,
                0.0f
            );

        bool m_orbiting = false;
        
    };
}