#include "Enemy/Enemy.h"
#include "Player/Player.h"

#include "Assets/AssetManager.h"
#include "Collision/Collider.h"
#include "Graphics/Model.h"
#include "Scene/Transform.h"
#include "Effects/Effects.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>

namespace MiraGame
{
    Enemy::Enemy(Player& player, MiraEngine::Scene& scene, const glm::vec3& spawnPosition)
        : Character("Enemy", 100)
        , m_player(player)
        , m_scene(scene)
        , m_spawnPosition(spawnPosition)
    {
        GetTransform().SetPosition(spawnPosition);
        GetTransform().SetRotation(glm::vec3(0.0f, m_spawnYaw, 0.0f));
		GetTransform().SetScale(glm::vec3(0.5f));

        SetModel("Assets/KayKit_Adventurers/Characters/fbx/Barbarian.fbx");
		SetTexture(MiraEngine::TextureSlot::Color, "Assets/KayKit_Adventurers/Characters/fbx/barbarian_texture.png");
		SetCollider(MiraEngine::Collider::Sphere(0.3f));

        auto model = GetModel();
        const std::string animations = "Assets/KayKit_Adventurers/Animations/fbx/Rig_Medium/";
        model->AddAnimations(MiraEngine::AssetManager::LoadAnimations(animations + "Rig_Medium_General.fbx"));
        model->AddAnimations(MiraEngine::AssetManager::LoadAnimations(animations + "Rig_Medium_MovementBasic.fbx"));
        model->AddAnimations(MiraEngine::AssetManager::LoadAnimations(animations + "Rig_Medium_CombatMelee.fbx"));

		PlayAnimation("Idle_A");
    }

    void Enemy::Update(float deltaTime)
    {
        MiraEngine::Transform& transform = GetTransform();

        if (IsDead())
        {
            ChangeState(EnemyState::Dead);

            return;
        }

        const glm::vec3 enemyPosition = transform.GetPosition();
        const glm::vec3 playerPosition = m_player.GetTransform().GetPosition();
        glm::vec3 toPlayer = playerPosition - enemyPosition;
        toPlayer.y = 0.0f;
        const float playerDistance = m_player.IsDead() ? std::numeric_limits<float>::max() : glm::length(toPlayer);

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
            else
            {
                RotateTowardsYaw(m_spawnYaw, deltaTime);
            }

            break;
        }

        case EnemyState::Chasing:
        {
            if (playerDistance > m_detectionRange)
            {
                ChangeState(EnemyState::Waiting);
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
                ChangeState(EnemyState::Waiting);
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

        case EnemyState::Waiting:
        {
            // Re-engage if the player comes back while we're still here.
            if (playerDistance <= m_detectionRange)
            {
                ChangeState(playerDistance <= m_stopDistance ? EnemyState::Attacking : EnemyState::Chasing);
                break;
            }

            m_waitTimer += deltaTime;

            if (m_waitTimer >= m_waitTime)
            {
                ChangeState(EnemyState::Returning);
            }

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

            float blend = 1.0f - std::clamp(spawnDistance / m_turnBlendDistance, 0.0f, 1.0f);
            blend = blend * blend;

            glm::vec3 rotation = transform.GetRotation();
            float moveYaw = rotation.y; // already set by FaceDirection in MoveTowards
            float delta = std::fmod(m_spawnYaw - moveYaw + 540.0f, 360.0f) - 180.0f;
            rotation.y = moveYaw + delta * blend;
            transform.SetRotation(rotation);

            break;
        }

        case EnemyState::Dead:
            break;
        }
    }

    void Enemy::ChangeState(EnemyState newState)
    {
        if (m_state == newState)
        {
            return;
        }

        m_state = newState;
        ResetAttack();
        m_waitTimer = 0.0f;

        switch (m_state)
        {
            case EnemyState::Idle:
                PlayAnimation("Idle_A");
                break;

            case EnemyState::Chasing:
                PlayAnimation("Walking_A");
                break;

            case EnemyState::Attacking:
                PlayAnimation("Idle_A");
                break;

            case EnemyState::Waiting:
                PlayAnimation("Idle_A");
                break;

            case EnemyState::Returning:
                PlayAnimation("Walking_A");
                break;

            case EnemyState::Dead:
                PlayAnimation("Death_B", false);
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
                PlayAnimation("Melee_1H_Attack_Stab", false);
            }
            return;
        }

        if (!m_attackHitApplied && m_attackTimer >= m_attackHitTime)
        {
            m_player.TakeDamage(m_attackDamage);
            SpawnHitSparks(m_scene, m_player.GetTransform().GetPosition() + glm::vec3(0.0f, 0.6f, 0.0f));
            std::cout << "Player HP: " << m_player.GetHealth() << "/" << m_player.GetMaxHealth() << "\n";
            m_attackHitApplied = true;
        }

        if (m_attackTimer >= m_attackDuration)
        {
            ResetAttack();
            PlayAnimation("Idle_A");
        }
    }

    void Enemy::ResetAttack()
    {
        m_attackInProgress = false;
        m_attackHitApplied = false;
        m_attackTimer = 0.0f;
    }

    void Enemy::RotateTowardsYaw(float targetYaw, float deltaTime)
    {
        MiraEngine::Transform& transform = GetTransform();
        glm::vec3 rotation = transform.GetRotation();

        // Shortest signed angle difference, in the range -180..180
        float delta = std::fmod(targetYaw - rotation.y + 540.0f, 360.0f) - 180.0f;

        float t = 1.0f - std::exp(-m_turnSharpness * deltaTime);
        rotation.y += delta * t;

        transform.SetRotation(rotation);
    }
}