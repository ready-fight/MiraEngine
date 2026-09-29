#pragma once

#include "GameObject.h"

namespace Mira
{
    class Enemy : public GameObject
    {
    public:
        Enemy(const std::shared_ptr<Model>& characterModel);
        void Move(const glm::vec3& direction, float deltaTime);

    private:
        float m_movementSpeed = 1.0f;
    };
}