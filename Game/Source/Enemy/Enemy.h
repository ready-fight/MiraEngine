#pragma once

#include "Character/Character.h"

namespace MiraGame
{
    class Player;

    class Enemy : public Character
    {
    public:
        explicit Enemy(Player* player);

        void Update(float deltaTime) override;

    private:
        Player* m_player = nullptr;

        float m_speed = 1.0f;
        float m_stopDistance = 1.5f;
        bool m_isMoving = false;
        bool m_hasDied = false;
        int m_attackDamage = 30;
        float m_attackCooldown = 1.0f;
        float m_attackTimer = 0.0f;
        bool m_isAttacking = false;
        bool m_attackHitApplied = false;
        float m_attackTime = 0.0f;
        float m_attackHitTime = 0.55f;
        float m_attackDuration = 0.6f;
    };
}