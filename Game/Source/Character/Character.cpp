#include "Character/Character.h"

#include <algorithm>
#include <cmath>
#include <glm/trigonometric.hpp>

namespace MiraGame
{
    Character::Character(const std::string& name, int maxHealth)
        : MiraEngine::GameObject(name)
        , m_maxHealth(maxHealth)
        , m_health(maxHealth)
    {
    }

    void Character::TakeDamage(int amount)
    {
        m_health = std::max(0, m_health - amount);
    }

    void Character::Heal(int amount)
    {
        m_health = std::min(m_maxHealth, m_health + amount);
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

    void Character::FaceDirection(const glm::vec3& direction)
    {
        if (direction.x == 0.0f && direction.z == 0.0f)
        {
            return;
        }

        glm::vec3 rotation = GetTransform().GetRotation();
        rotation.y = glm::degrees(std::atan2(direction.x, direction.z));
        GetTransform().SetRotation(rotation);
    }

    glm::vec3 Character::GetFacing() const
    {
        const float yaw = glm::radians(GetTransform().GetRotation().y);
        return glm::vec3(std::sin(yaw), 0.0f, std::cos(yaw));
    }
}