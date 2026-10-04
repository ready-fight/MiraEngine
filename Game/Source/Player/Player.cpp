#include "Player/Player.h"

#include "Core/Input.h"
#include "Scene/Transform.h"

#include "Scene/Scene.h"
#include "Enemy/Enemy.h"
#include "UI/UI.h"

#include "Assets/AssetManager.h"
#include "Animation/Animator.h"
#include "Item/Item.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>

namespace MiraGame
{
	Player::Player(MiraEngine::Scene* scene)
		: Character("Player", 100),
		m_scene(scene)
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

	void Player::Update(float deltaTime)
	{

		if (IsDead())
		{
			if (!m_hasDied)
			{
				m_isAttacking = false;
				m_attackHitApplied = false;

				if (MiraEngine::Animator* animator = GetAnimator())
				{
					animator->PlayAnimation(3, false);
				}

				m_hasDied = true;
			}

			return;
		}

		glm::vec3 movement(0.0f);

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::W))
			movement.z += 1.0f;

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::S))
			movement.z -= 1.0f;

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::A))
			movement.x += 1.0f;

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::D))
			movement.x -= 1.0f;

		const bool isMoving =
			glm::length(movement) > 0.0f;

		const bool attackDown =
			MiraEngine::Input::IsKeyPressed(
				MiraEngine::Key::E
			);

		// Start attack.
		if (
			attackDown &&
			!m_wasAttackDown &&
			!m_isAttacking
			)
		{
			m_isAttacking = true;
			m_attackHitApplied = false;
			m_attackTime = 0.0f;

			if (MiraEngine::Animator* animator = GetAnimator())
			{
				animator->PlayAnimation(2, false);
			}
		}

		m_wasAttackDown = attackDown;

		// Attack is currently playing.
		if (m_isAttacking)
		{
			m_attackTime += deltaTime;

			if (
				!m_attackHitApplied &&
				m_attackTime >= m_attackHitTime
				)
			{
				const MiraEngine::Transform& transform =
					GetTransform();

				const float yaw =
					glm::radians(
						transform.GetRotation().y
					);

				const glm::vec3 forward(
					std::sin(yaw),
					0.0f,
					std::cos(yaw)
				);

				const glm::vec3 attackCenter =
					transform.GetPosition() +
					forward * m_attackOffset;

				if (m_scene)
				{
					for (const auto& object : m_scene->GetObjects())
					{
						MiraGame::Enemy* enemy =
							dynamic_cast<MiraGame::Enemy*>(
								object.get()
								);

						if (!enemy || enemy->IsDead())
						{
							continue;
						}

						const float distance =
							glm::distance(
								attackCenter,
								enemy->GetTransform().GetPosition()
							);

						if (distance <= m_attackRadius)
						{
							enemy->TakeDamage(
								m_attackDamage
							);

							std::cout
								<< "Enemy HP: "
								<< enemy->GetHealth()
								<< "/"
								<< enemy->GetMaxHealth()
								<< "\n";
						}
					}
				}

				m_attackHitApplied = true;
			}

			if (m_attackTime >= m_attackDuration)
			{
				m_isAttacking = false;
				m_isMoving = isMoving;

				if (MiraEngine::Animator* animator = GetAnimator())
				{
					animator->PlayAnimation(
						isMoving ? 1 : 0
					);
				}
			}

			return;
		}

		// Idle / walking animation.
		if (isMoving != m_isMoving)
		{
			if (MiraEngine::Animator* animator = GetAnimator())
			{
				animator->PlayAnimation(
					isMoving ? 1 : 0
				);
			}

			m_isMoving = isMoving;
		}

		if (isMoving)
		{
			movement = glm::normalize(movement);

			MiraEngine::Transform& transform =
				GetTransform();

			const float yaw =
				std::atan2(
					movement.x,
					movement.z
				) * 180.0f / 3.14159265f;

			glm::vec3 rotation =
				transform.GetRotation();

			rotation.y = yaw;

			transform.SetRotation(rotation);

			transform.SetPosition(
				transform.GetPosition() +
				movement * m_speed * deltaTime
			);
		}

		const bool interactDown =
			MiraEngine::Input::IsKeyPressed(
				MiraEngine::Key::F
			);

		if (interactDown && !m_wasInteractDown)
		{
			if (m_scene)
			{
				for (const auto& object : m_scene->GetObjects())
				{
					MiraGame::Item* item =
						dynamic_cast<MiraGame::Item*>(
							object.get()
							);

					if (!item || item->IsPickedUp())
					{
						continue;
					}

					const float distance =
						glm::distance(
							GetTransform().GetPosition(),
							item->GetTransform().GetPosition()
						);

					std::cout << distance << "\n";

					if (distance <= m_interactRange)
					{
						item->Pickup();

						std::cout
							<< "Picked up item\n";

						break;
					}
				}
			}
		}

		m_wasInteractDown = interactDown;
	}

	void Player::DrawUI()
	{
		MiraEngine::UI::BeginWindow("Player");

		const float healthPercent =
			static_cast<float>(GetHealth()) /
			static_cast<float>(GetMaxHealth());

		const std::string healthText =
			std::to_string(GetHealth()) +
			" / " +
			std::to_string(GetMaxHealth());

		MiraEngine::UI::DrawProgressBar(
			healthPercent,
			healthText
		);

		MiraEngine::UI::EndWindow();
	}
}