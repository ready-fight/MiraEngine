#include "Player/Player.h"

#include "Core/Input.h"
#include "Scene/Transform.h"
#include "Collision/SphereCollider.h"

#include "Scene/Scene.h"
#include "Enemy/Enemy.h"
#include "UI/UI.h"
#include "Item/Item.h"

#include "Assets/AssetManager.h"
#include "Animation/Animator.h"
#include "Interaction/Interactable.h"

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

		SetModel(model);

		auto idle =
			MiraEngine::AssetManager::GetAnimation("Idle");

		if (idle) {
			model->AddAnimation(idle);
		}

		auto walking =
			MiraEngine::AssetManager::GetAnimation("Walking");

		if (walking) {
			model->AddAnimation(walking);
		}

		auto running =
			MiraEngine::AssetManager::GetAnimation("Running");

		if (running) {
			model->AddAnimation(running);
		}

		auto attack =
			MiraEngine::AssetManager::GetAnimation("Attack");

		if (attack) {
			model->AddAnimation(attack);
		}

		auto death =
			MiraEngine::AssetManager::GetAnimation("Death");

		if (death) {
			model->AddAnimation(death);
		}

		/*auto idle =
			MiraEngine::AssetManager::GetAnimation("Idle");

		if (idle) {
			model->AddAnimation(idle);
		}

		auto walking =
			MiraEngine::AssetManager::GetAnimation("Walking");

		if (walking) {
			model->AddAnimation(walking);
		}

		auto attack =
			MiraEngine::AssetManager::GetAnimation("Attack");

		if (attack) {
			model->AddAnimation(attack);
		}

		auto death =
			MiraEngine::AssetManager::GetAnimation("Death");

		if (death) {
			model->AddAnimation(death);
		}*/

		SetCollider(
			std::make_unique<MiraEngine::SphereCollider>(
				0.3f
			)
		);


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
					animator->PlayAnimation(4, false);
				}

				m_hasDied = true;
			}

			return;
		}

		glm::vec2 movementInput(0.0f);

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::W))
			movementInput.y += 1.0f;

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::S))
			movementInput.y -= 1.0f;

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::A))
			movementInput.x -= 1.0f;

		if (MiraEngine::Input::IsKeyPressed(MiraEngine::Key::D))
			movementInput.x += 1.0f;

		const bool isMoving =
			glm::length(movementInput) > 0.0f;

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
				animator->PlayAnimation(3, false);
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
							int attackDamage = 5;

							if (m_equippedWeapon.has_value())
							{
								attackDamage =
									m_equippedWeapon->damage;
							}

							enemy->TakeDamage(
								attackDamage
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
						isMoving ? 2 : 0
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
					isMoving ? 2 : 0
				);
			}

			m_isMoving = isMoving;
		}

		if (isMoving)
		{
			glm::vec3 movement =
				MiraEngine::Input::GetMovementDirection(
					movementInput
				);

			MiraEngine::Transform& transform =
				GetTransform();

			const float yaw =
				glm::degrees(
					std::atan2(
						movement.x,
						movement.z
					)
				);

			glm::vec3 rotation =
				transform.GetRotation();

			rotation.y = yaw;

			transform.SetRotation(rotation);

			if (m_scene)
			{
				m_scene->TryMove(
					*this,
					movement * m_speed * deltaTime
				);
			}
		}

		const bool interactDown =
			MiraEngine::Input::IsKeyPressed(
				MiraEngine::Key::F
			);

		if (interactDown && !m_wasInteractDown)
		{
			if (m_scene)
			{
				Interactable* closestInteractable = nullptr;
				float closestDistance = m_interactRange;

				for (const auto& object : m_scene->GetObjects())
				{
					Interactable* interactable =
						dynamic_cast<Interactable*>(
							object.get()
							);

					if (
						!interactable ||
						!interactable->CanInteract()
						)
					{
						continue;
					}

					const float distance =
						glm::distance(
							GetTransform().GetPosition(),
							object->GetTransform().GetPosition()
						);

					if (distance <= closestDistance)
					{
						closestDistance = distance;
						closestInteractable = interactable;
					}
				}

				if (closestInteractable)
				{
					closestInteractable->Interact(
						*this
					);
				}
			}
		}

		m_wasInteractDown = interactDown;


		if (m_questStarted) {
			for (auto& object : m_scene->GetObjects())
			{
				Item* item = dynamic_cast<Item*>(object.get());

				if (!item) { continue; }

				if (item->GetName() == "Dungeon Key" && !item->IsPickedUp()) {
					item->FadeIn(deltaTime);
					break;
				}
			}
		}
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

		MiraEngine::UI::DrawText("HP:");

		MiraEngine::UI::DrawProgressBar(
			healthPercent,
			healthText
		);

		MiraEngine::UI::DrawText("Equipped:");

		if (m_equippedWeapon.has_value())
		{
			MiraEngine::UI::DrawText(
				m_equippedWeapon->name +
				" (Damage: " +
				std::to_string(
					m_equippedWeapon->damage
				) +
				")"
			);
		}
		else
		{
			MiraEngine::UI::DrawText("None");
		}

		if (MiraEngine::UI::DrawButton("Unequip"))
		{
			UnequipWeapon();
		}

		MiraEngine::UI::DrawText("Inventory:");

		const auto& items =
			m_inventory.GetItems();

		for (std::size_t i = 0; i < items.size(); ++i)
		{
			const ItemData& item =
				items[i];

			MiraEngine::UI::DrawText(
				"- " + item.name
			);

			if (item.type == ItemType::Weapon)
			{
				if (
					MiraEngine::UI::DrawButton(
						"Equip##" +
						std::to_string(i)
					)
					)
				{
					EquipItem(i);
				}
			}

			if (item.type == ItemType::Consumable)
			{
				if (
					MiraEngine::UI::DrawButton(
						"Use##" +
						std::to_string(i)
					)
					)
				{
					UseItem(i);
					break;
				}
			}
		}

		MiraEngine::UI::DrawText("Quests:");

		for (const Quest& quest : m_questSystem.GetQuests())
		{
			if (quest.status == QuestStatus::Inactive)
			{
				continue;
			}

			std::string status;

			if (quest.status == QuestStatus::Active)
			{
				status = "Active";
			}
			else if (quest.status == QuestStatus::Completed)
			{
				status = "Completed";
			}

			else if (quest.status == QuestStatus::TurnedIn)
			{
				status = "Turned In";
			}

			MiraEngine::UI::DrawText(
				quest.name +
				" [" +
				status +
				"]"
			);

			MiraEngine::UI::DrawText(
				quest.description
			);
		}

		MiraEngine::UI::EndWindow();
	}

	void Player::EquipItem(std::size_t index)
	{
		const auto& items =
			m_inventory.GetItems();

		if (index >= items.size())
		{
			return;
		}

		const ItemData& item =
			items[index];

		if (item.type != ItemType::Weapon)
		{
			return;
		}

		m_equippedWeapon = item;

		std::cout
			<< "Equipped: "
			<< item.name
			<< "\n";
	}

	void Player::UnequipWeapon()
	{
		if (!m_equippedWeapon.has_value())
		{
			return;
		}

		std::cout
			<< "Unequipped: "
			<< m_equippedWeapon->name
			<< "\n";

		m_equippedWeapon.reset();
	}

	void Player::UseItem(std::size_t index)
	{
		const auto& items =
			m_inventory.GetItems();

		if (index >= items.size())
		{
			return;
		}

		const ItemData item =
			items[index];

		if (item.type != ItemType::Consumable && item.type != ItemType::KeyItem)
		{
			return;
		}

		Heal(item.healAmount);

		m_inventory.RemoveItem(index);

		std::cout
			<< "Used: "
			<< item.name
			<< "\n";
	}

	void Player::AddItem(
		const ItemData& item
	)
	{
		m_inventory.AddItem(item);

		std::cout
			<< "Picked up: "
			<< item.name
			<< "\n";

		m_questSystem.OnItemAdded(
			item.name
		);
	}

	bool Player::HasItem(
		const std::string& itemName
	) const
	{
		for (const ItemData& item : m_inventory.GetItems())
		{
			if (item.name == itemName)
			{
				return true;
			}
		}

		return false;
	}

	bool Player::RemoveItem(
		const std::string& itemName
	)
	{
		return m_inventory.RemoveItem(
			itemName
		);
	}

	bool Player::StartQuest(
		const std::string& questName
	)
	{
		if (!m_questSystem.StartQuest(questName))
		{
			return false;
		}

		m_questStarted = true;

		const Quest* quest =
			m_questSystem.GetQuest(questName);

		if (
			quest &&
			!quest->requiredItem.empty() &&
			HasItem(quest->requiredItem)
			)
		{
			m_questSystem.OnItemAdded(
				quest->requiredItem
			);
		}

		return true;
	}

	bool Player::CompleteQuest(
		const std::string& questName
	)
	{
		return m_questSystem.CompleteQuest(
			questName
		);
	}

	const Quest* Player::GetQuest(
		const std::string& questName
	) const
	{
		return m_questSystem.GetQuest(
			questName
		);
	}

	bool Player::TurnInQuest(
		const std::string& questName
	)
	{
		return m_questSystem.TurnInQuest(
			questName
		);
	}
}