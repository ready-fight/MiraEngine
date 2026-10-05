#include "Character/Character.h"

#include <algorithm>

namespace MiraGame
{
    Character::Character(
        const std::string& name,
        int maxHealth
    )
        : MiraEngine::GameObject(name),
        m_maxHealth(maxHealth),
        m_health(maxHealth)
    {
    }

    void Character::TakeDamage(int amount)
    {
        m_health = std::max(
            0,
            m_health - amount
        );
    }

    void Character::Heal(int amount)
    {
        m_health += amount;

        if (m_health > m_maxHealth)
        {
            m_health = m_maxHealth;
        }
    }

    int Character::GetHealth() const
    {
        return m_health;
    }

    int Character::GetMaxHealth() const
    {
        return m_maxHealth;
    }

    bool Character::IsDead() const
    {
        return m_health <= 0;
    }
}