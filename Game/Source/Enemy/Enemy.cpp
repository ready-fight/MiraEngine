#include "Enemy/Enemy.h"
#include "Player/Player.h"

#include "Animation/Animator.h"
#include "Assets/AssetManager.h"
#include "Collision/SphereCollider.h"
#include "Graphics/Model.h"
#include "Scene/Transform.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <iostream>

namespace MiraGame {
    Enemy::Enemy(Player* player, MiraEngine::Scene* scene)
        : Character("Enemy", 100), m_player(player), m_scene(scene) {
        auto model =
            MiraEngine::AssetManager::LoadModel("Assets/Models/Idle.fbx");

        SetModel(model);

        SetCollider(std::make_unique<MiraEngine::SphereCollider>(0.3f));

        GetTransform().SetScale(glm::vec3(0.01f));

        if (MiraEngine::Animator* animator = GetAnimator()) {
            animator->PlayAnimation("Idle");
        }
    }

    void Enemy::ChangeState(EnemyState newState) {
        if (m_state == newState) {
            return;
        }

        m_state = newState;

        m_attackInProgress = false;
        m_attackHitApplied = false;
        m_attackTime = 0.0f;
        m_attackTimer = 0.0f;

        MiraEngine::Animator* animator = GetAnimator();

        if (!animator) {
            return;
        }

        switch (m_state) {
        case EnemyState::Idle:
            animator->PlayAnimation("Idle");
            break;

        case EnemyState::Chasing:
            animator->PlayAnimation("Walking");
            break;

        case EnemyState::Attacking:
            animator->PlayAnimation("Idle");
            break;

        case EnemyState::Returning:
            animator->PlayAnimation("Walking");
            break;

        case EnemyState::Dead:
            animator->PlayAnimation("Death", false);
            break;
        }
    }

    void Enemy::MoveTowards(const glm::vec3& target, float deltaTime) {
        MiraEngine::Transform& transform = GetTransform();

        glm::vec3 direction = target - transform.GetPosition();

        direction.y = 0.0f;

        if (glm::length(direction) <= 0.0f) {
            return;
        }

        direction = glm::normalize(direction);

        if (m_scene) {
            m_scene->TryMove(*this, direction * m_speed * deltaTime);
        }

        const float yaw = glm::degrees(std::atan2(direction.x, direction.z));

        glm::vec3 rotation = transform.GetRotation();

        rotation.y = yaw;

        transform.SetRotation(rotation);
    }

    void Enemy::Update(float deltaTime) {
        MiraEngine::Transform& transform = GetTransform();

        if (!m_spawnPositionInitialized) {
            m_spawnPosition = transform.GetPosition();

            m_spawnPositionInitialized = true;
        }

        if (IsDead()) {
            ChangeState(EnemyState::Dead);

            return;
        }

        if (!m_player) {
            return;
        }

        if (m_player->IsDead()) {
            ChangeState(EnemyState::Idle);

            return;
        }

        const glm::vec3 enemyPosition = transform.GetPosition();

        const glm::vec3 playerPosition = m_player->GetTransform().GetPosition();

        glm::vec3 toPlayer = playerPosition - enemyPosition;

        toPlayer.y = 0.0f;

        const float playerDistance = glm::length(toPlayer);

        switch (m_state) {
        case EnemyState::Idle: {
            if (playerDistance <= m_stopDistance) {
                ChangeState(EnemyState::Attacking);
            } else if (playerDistance <= m_detectionRange) {
                ChangeState(EnemyState::Chasing);
            }

            break;
        }

        case EnemyState::Chasing: {
            if (playerDistance > m_detectionRange) {
                ChangeState(EnemyState::Returning);

                break;
            }

            if (playerDistance <= m_stopDistance) {
                ChangeState(EnemyState::Attacking);

                break;
            }

            MoveTowards(playerPosition, deltaTime);

            break;
        }

        case EnemyState::Attacking: {
            if (playerDistance > m_detectionRange) {
                ChangeState(EnemyState::Returning);

                break;
            }

            if (playerDistance > m_stopDistance) {
                ChangeState(EnemyState::Chasing);

                break;
            }

            if (m_attackInProgress) {
                m_attackTime += deltaTime;

                if (!m_attackHitApplied && m_attackTime >= m_attackHitTime) {
                    m_player->TakeDamage(m_attackDamage);

                    std::cout << "Player HP: " << m_player->GetHealth() << "/"
                              << m_player->GetMaxHealth() << "\n";

                    m_attackHitApplied = true;
                }

                if (m_attackTime >= m_attackDuration) {
                    m_attackInProgress = false;

                    m_attackHitApplied = false;

                    m_attackTime = 0.0f;

                    m_attackTimer = 0.0f;

                    if (MiraEngine::Animator* animator = GetAnimator()) {
                        animator->PlayAnimation("Idle");
                    }
                }

                break;
            }

            m_attackTimer += deltaTime;

            if (m_attackTimer >= m_attackCooldown) {
                m_attackTimer = 0.0f;

                m_attackTime = 0.0f;

                m_attackHitApplied = false;

                m_attackInProgress = true;

                if (MiraEngine::Animator* animator = GetAnimator()) {
                    animator->PlayAnimation("Attack", false);
                }
            }

            break;
        }

        case EnemyState::Returning: {
            if (playerDistance <= m_detectionRange) {
                if (playerDistance <= m_stopDistance) {
                    ChangeState(EnemyState::Attacking);
                } else {
                    ChangeState(EnemyState::Chasing);
                }

                break;
            }

            glm::vec3 toSpawn = m_spawnPosition - enemyPosition;

            toSpawn.y = 0.0f;

            const float spawnDistance = glm::length(toSpawn);

            if (spawnDistance <= m_returnStopDistance) {
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