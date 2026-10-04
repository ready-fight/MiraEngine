#pragma once

#include "Character/Character.h"
#include <cmath>

namespace MiraEngine
{
    class Scene;
}

namespace MiraGame
{
    class Player : public Character
    {
    public:
        explicit Player(MiraEngine::Scene* scene);

        void Update(float deltaTime) override;

    private:
        float m_speed = 4.0f;
        bool m_isMoving = false;

        float m_attackOffset = 1.0f;
        float m_attackRadius = 0.8f;
        int m_attackDamage = 20;

        bool m_wasAttackDown = false;
        bool m_isAttacking = false;
        bool m_attackHitApplied = false;
        bool m_hasDied = false;

        float m_attackTime = 0.0f;
        float m_attackHitTime = 0.55f;
        float m_attackDuration = 0.6f;

        MiraEngine::Scene* m_scene = nullptr;
    };
}