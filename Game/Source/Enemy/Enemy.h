#pragma once

#include "Character/Character.h"
#include "Scene/Scene.h"

#include <glm/vec3.hpp>

namespace MiraGame
{
    class Player;

    class Enemy : public Character
    {
    public:
        explicit Enemy(Player& player, MiraEngine::Scene& scene, const glm::vec3& spawnPosition);
        void Update(float deltaTime) override;

    private:
        enum class EnemyState
        {
            Idle,
            Chasing,
            Attacking,
            Waiting,
            Returning,
            Dead
        };

        void ChangeState(EnemyState newState);
        void MoveTowards(const glm::vec3& target, float deltaTime);
        void UpdateAttack(float deltaTime);
        void ResetAttack();
        void RotateTowardsYaw(float targetYaw, float deltaTime);

        Player& m_player;
        MiraEngine::Scene& m_scene;
        EnemyState m_state = EnemyState::Idle;
        float m_speed = 2.5f;
        float m_stopDistance = 1.5f;
        float m_detectionRange = 6.0f;
        glm::vec3 m_spawnPosition;
        float m_spawnYaw = 180.0f;
        float m_turnSharpness = 7.0f;
        float m_turnBlendDistance = 0.75f;
        float m_returnStopDistance = 0.1f;
        int m_attackDamage = 5;
        float m_attackCooldown = 1.0f;
        float m_attackTimer = 0.0f;
        bool m_attackInProgress = false;
        bool m_attackHitApplied = false;
        float m_attackHitTime = 0.55f;
        float m_attackDuration = 0.6f;
        float m_waitTime = 0.5f;
        float m_waitTimer = 0.0f;
    };
}