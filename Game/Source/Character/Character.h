#pragma once

#include "Scene/GameObject.h"

#include <string>

namespace MiraGame
{
    class Character : public MiraEngine::GameObject
    {
    public:
        Character(
            const std::string& name,
            int maxHealth
        );

        void TakeDamage(int amount);

        int GetHealth() const;
        int GetMaxHealth() const;

        bool IsDead() const;

    private:
        int m_maxHealth = 100;
        int m_health = 100;
    };
}