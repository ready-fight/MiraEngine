#include "Enemy/Enemy.h"
#include "Player/Player.h"

#include "Assets/AssetManager.h"
#include "Collision/Collider.h"
#include "Graphics/Model.h"
#include "Scene/Transform.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <iostream>

namespace MiraGame
{
    Enemy::Enemy(Player& player, MiraEngine::Scene& scene, const glm::vec3& spawnPosition)
        : Character("Enemy", 100)
        , m_player(player)
        , m_scene(scene)
        , m_spawnPosition(spawnPosition)
    {
        GetTransform().SetPosition(spawnPosition);

        SetModel("Assets/Models/Idle.fbx");
        auto model = GetModel();

        auto idle = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Idle.fbx");
        auto walking = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Walking.fbx");
        auto running = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Running.fbx");
        auto attack = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Attack.fbx");
        auto death = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Death.fbx");
        model->AddAnimation(idle);
        model->AddAnimation(walking);
        model->AddAnimation(running);
        model->AddAnimation(attack);
        model->AddAnimation(death);

        SetCollider(MiraEngine::Collider::Sphere(0.3f));
        GetTransform().SetScale(glm::vec3(0.01f));

        PlayAnimation("Idle");
    }

    void Enemy::ChangeState(EnemyState newState)
    {
        if (m_state == newState)
        {
            return;
        }

        m_state = newState;
        ResetAttack();

        switch (m_state)
        {
            case EnemyState::Idle:
                PlayAnimation("Idle");
                break;

            case EnemyState::Chasing:
                PlayAnimation("Walking");
                break;

            case EnemyState::Attacking:
                PlayAnimation("Idle");
                break;

            case EnemyState::Returning:
                PlayAnimation("Walking");
                break;

            case EnemyState::Dead:
                PlayAnimation("Death", false);
				SetCollider(MiraEngine::Collider::Sphere(0.0f));
                break;
        }
    }

    void Enemy::MoveTowards(const glm::vec3& target, float deltaTime)
    {
        MiraEngine::Transform& transform = GetTransform();
        glm::vec3 direction = target - transform.GetPosition();
        direction.y = 0.0f;

        if (glm::length(direction) <= 0.0f)
        {
            return;
        }

        direction = glm::normalize(direction);

        m_scene.TryMove(*this, direction * m_speed * deltaTime);
        
		FaceDirection(direction);
    }

    void Enemy::UpdateAttack(float deltaTime)
    {
        m_attackTimer += deltaTime;

        if (!m_attackInProgress)
        {
            if (m_attackTimer >= m_attackCooldown)
            {
                m_attackTimer = 0.0f;
                m_attackInProgress = true;
                PlayAnimation("Attack", false);
            }
            return;
        }

        if (!m_attackHitApplied && m_attackTimer >= m_attackHitTime)
        {
            m_player.TakeDamage(m_attackDamage);
            std::cout << "Player HP: " << m_player.GetHealth() << "/" << m_player.GetMaxHealth() << "\n";
            m_attackHitApplied = true;
        }

        if (m_attackTimer >= m_attackDuration)
        {
            ResetAttack();
            PlayAnimation("Idle");
        }
    }

    void Enemy::ResetAttack()
    {
        m_attackInProgress = false;
        m_attackHitApplied = false;
        m_attackTimer = 0.0f;
    }

    void Enemy::Update(float deltaTime)
    {
        MiraEngine::Transform& transform = GetTransform();

        if (IsDead())
        {
            ChangeState(EnemyState::Dead);

            return;
        }

        if (m_player.IsDead())
        {
            ChangeState(EnemyState::Idle);

            return;
        }

        const glm::vec3 enemyPosition = transform.GetPosition();
        const glm::vec3 playerPosition = m_player.GetTransform().GetPosition();
        glm::vec3 toPlayer = playerPosition - enemyPosition;
        toPlayer.y = 0.0f;
        const float playerDistance = glm::length(toPlayer);

        switch (m_state)
        {
            case EnemyState::Idle:
            {
                if (playerDistance <= m_stopDistance)
                {
                    ChangeState(EnemyState::Attacking);
                }
                else if (playerDistance <= m_detectionRange)
                {
                    ChangeState(EnemyState::Chasing);
                }

                break;
            }

            case EnemyState::Chasing:
            {
                if (playerDistance > m_detectionRange)
                {
                    ChangeState(EnemyState::Returning);

                    break;
                }

                if (playerDistance <= m_stopDistance)
                {
                    ChangeState(EnemyState::Attacking);

                    break;
                }

                MoveTowards(playerPosition, deltaTime);

                break;
            }

            case EnemyState::Attacking:
            {
                if (playerDistance > m_detectionRange)
                {
                    ChangeState(EnemyState::Returning);
                    break;
                }

                if (playerDistance > m_stopDistance)
                {
                    ChangeState(EnemyState::Chasing);
                    break;
                }

                FaceDirection(toPlayer);
                UpdateAttack(deltaTime);
                break;
            }

            case EnemyState::Returning:
            {
                if (playerDistance <= m_detectionRange)
                {
                    if (playerDistance <= m_stopDistance)
                    {
                        ChangeState(EnemyState::Attacking);
                    }
                    else
                    {
                        ChangeState(EnemyState::Chasing);
                    }

                    break;
                }

                glm::vec3 toSpawn = m_spawnPosition - enemyPosition;
                toSpawn.y = 0.0f;
                const float spawnDistance = glm::length(toSpawn);

                if (spawnDistance <= m_returnStopDistance)
                {
                    ChangeState(EnemyState::Idle);

                    break;
                }

                MoveTowards(m_spawnPosition, deltaTime);

                break;
            }

            case EnemyState::Dead:
                break;
        }
    }
}