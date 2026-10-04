#include "Enemy/Enemy.h"
#include "Player/Player.h"

#include "Assets/AssetManager.h"
#include "Animation/Animator.h"
#include "Scene/Transform.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <iostream>

namespace MiraGame
{
    Enemy::Enemy(Player* player)
        : Character("Enemy", 100),
        m_player(player)
    {
        auto model =
            MiraEngine::AssetManager::LoadModel(
                "Assets/Models/Idle.fbx"
            );

        if (model->GetAnimations().size() < 2)
        {
            model->LoadAnimation(
                "Assets/Models/Walking.fbx"
            );
        }

        if (model->GetAnimations().size() < 3)
        {
            model->LoadAnimation(
                "Assets/Models/Attack.fbx"
            );
        }

        if (model->GetAnimations().size() < 4)
        {
            model->LoadAnimation(
                "Assets/Models/Death.fbx"
            );
        }

        SetModel(model);

        GetTransform().SetScale(
            glm::vec3(0.01f)
        );


        if (MiraEngine::Animator* animator = GetAnimator())
        {
            animator->PlayAnimation(0);
        }
    }

    void Enemy::Update(float deltaTime)
    {

        if (IsDead())
        {
            if (!m_hasDied)
            {
                if (MiraEngine::Animator* animator = GetAnimator())
                {
                    animator->PlayAnimation(3, false); // Death

                }

                m_hasDied = true;
            }

            return;
        }

        if (!m_player)
        {
            return;
        }

        MiraEngine::Transform& transform =
            GetTransform();

        const glm::vec3 enemyPosition =
            transform.GetPosition();

        const glm::vec3 playerPosition =
            m_player->GetTransform().GetPosition();

        glm::vec3 direction =
            playerPosition - enemyPosition;

        direction.y = 0.0f;

        const float distance =
            glm::length(direction);

        const bool isMoving =
            distance > m_stopDistance;

        // Change animation only when state changes.
        if (isMoving != m_isMoving)
        {
            if (MiraEngine::Animator* animator = GetAnimator())
            {
                if (isMoving)
                {
                    animator->PlayAnimation(1); // Walking
                }
                else
                {
                    animator->PlayAnimation(0); // Idle
                }
            }

            m_isMoving = isMoving;
        }

        if (!isMoving)
        {
            if (m_player->IsDead())
            {
                return;
            }

            // Currently attacking.
            if (m_isAttacking)
            {
                m_attackTime += deltaTime;

                if (
                    !m_attackHitApplied &&
                    m_attackTime >= m_attackHitTime
                    )
                {
                    m_player->TakeDamage(
                        m_attackDamage
                    );

                    std::cout
                        << "Player HP: "
                        << m_player->GetHealth()
                        << "/"
                        << m_player->GetMaxHealth()
                        << "\n";

                    m_attackHitApplied = true;
                }

                if (m_attackTime >= m_attackDuration)
                {
                    m_isAttacking = false;

                    if (MiraEngine::Animator* animator = GetAnimator())
                    {
                        animator->PlayAnimation(0); // Idle
                    }
                }

                return;
            }

            // Waiting for next attack.
            m_attackTimer += deltaTime;

            if (m_attackTimer >= m_attackCooldown)
            {
                m_attackTimer = 0.0f;
                m_attackTime = 0.0f;
                m_attackHitApplied = false;
                m_isAttacking = true;

                if (MiraEngine::Animator* animator = GetAnimator())
                {
                    animator->PlayAnimation(2, false); // Attack
                }
            }

            return;
        }

        m_attackTimer = 0.0f;

        m_attackTimer = 0.0f;

        direction = glm::normalize(direction);

        transform.SetPosition(
            enemyPosition +
            direction * m_speed * deltaTime
        );

        const float yaw =
            std::atan2(direction.x, direction.z) *
            180.0f / 3.14159265f;

        glm::vec3 rotation =
            transform.GetRotation();

        rotation.y = yaw;

        transform.SetRotation(rotation);
    }
}