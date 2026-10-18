#include "Player/Player.h"
#include "Graphics/Model.h"

#include "Collision/Collider.h"
#include "Core/Input.h"
#include "Scene/Transform.h"

#include "Enemy/Enemy.h"
#include "Item/Item.h"
#include "Scene/Scene.h"
#include "UI/UI.h"

#include "Assets/AssetManager.h"
#include "Interaction/Interactable.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>
#include <optional>

namespace MiraGame
{
	Player::Player(MiraEngine::Scene& scene)
		: Character("Player", 100)
		, m_scene(scene)
	{
		SetModel("Assets/KayKit_Adventurers/Characters/fbx/Knight.fbx");
		SetTexture(MiraEngine::TextureSlot::Color, "Assets/KayKit_Adventurers/Characters/fbx/knight_texture.png");

		auto model = GetModel();
		model->AddAnimation(MiraEngine::AssetManager::LoadAnimation("Assets/KayKit_Adventurers/Animations/fbx/Rig_Medium/Rig_Medium_General.fbx", "Idle_B"));
		model->AddAnimation(MiraEngine::AssetManager::LoadAnimation("Assets/KayKit_Adventurers/Animations/fbx/Rig_Medium/Rig_Medium_General.fbx", "Death_A"));
		model->AddAnimation(MiraEngine::AssetManager::LoadAnimation("Assets/KayKit_Adventurers/Animations/fbx/Rig_Medium/Rig_Medium_MovementBasic.fbx", "Running_A"));
		model->AddAnimation(MiraEngine::AssetManager::LoadAnimation("Assets/KayKit_Adventurers/Animations/fbx/Rig_Medium/Rig_Medium_CombatMelee.fbx", "Melee_Unarmed_Attack_Punch_A"));

		SetCollider(MiraEngine::Collider::Sphere(0.3f));
		GetTransform().SetScale(glm::vec3(0.5f));
		PlayAnimation("Idle_B");
	}

	void Player::Update(float deltaTime)
	{
		if (IsDead())
		{
			if (!m_hasDied)
			{
				m_isAttacking = false;
				m_attackHitApplied = false;

				FadeOut(deltaTime);

				PlayAnimation("Death_A", false);

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

		const bool isMoving = glm::length(movementInput) > 0.0f;
		const bool attackDown = MiraEngine::Input::IsKeyPressed(MiraEngine::Key::E);

		// Start attack.
		if (attackDown && !m_wasAttackDown && !m_isAttacking)
		{
			m_isAttacking = true;
			m_attackHitApplied = false;
			m_attackTime = 0.0f;

			PlayAnimation("Melee_Unarmed_Attack_Punch_A", false);
		}

		m_wasAttackDown = attackDown;

		// Attack is currently playing.
		if (m_isAttacking)
		{
			m_attackTime += deltaTime;

			if (!m_attackHitApplied && m_attackTime >= m_attackHitTime)
			{
				const glm::vec3 attackCenter = GetTransform().GetPosition() + GetFacing() * m_attackOffset;


				for (const auto& object : m_scene.GetObjects())
				{
					MiraGame::Enemy* enemy = dynamic_cast<MiraGame::Enemy*>(object.get());

					if (!enemy || enemy->IsDead())
					{
						continue;
					}

					const float distance = glm::distance(attackCenter, enemy->GetTransform().GetPosition());

					if (distance <= m_attackRadius)
					{
						int attackDamage = 5;

						if (m_equippedWeapon.has_value())
						{
							attackDamage = m_equippedWeapon->damage;
						}

						enemy->TakeDamage(attackDamage);
						std::cout << "Enemy HP: " << enemy->GetHealth() << "/" << enemy->GetMaxHealth() << "\n";
					}
				}

				m_attackHitApplied = true;
			}

			if (m_attackTime >= m_attackDuration)
			{
				m_isAttacking = false;
				m_isMoving = isMoving;

				PlayAnimation(isMoving ? "Running_A" : "Idle_B");
			}

			return;
		}

		// Idle / walking animation.
		if (isMoving != m_isMoving)
		{
			PlayAnimation(isMoving ? "Running_A" : "Idle_B");

			m_isMoving = isMoving;
		}

		if (isMoving)
		{
			const glm::vec3 movement = MiraEngine::Input::GetMovementDirection(movementInput);
			FaceDirection(movement);

			m_scene.TryMove(*this, movement * m_speed * deltaTime);
		}

		const bool interactDown = MiraEngine::Input::IsKeyPressed(MiraEngine::Key::F);

		if (interactDown && !m_wasInteractDown)
		{

			Interactable* closestInteractable = nullptr;
			float closestDistance = m_interactRange;

			for (const auto& object : m_scene.GetObjects())
			{
				Interactable* interactable = dynamic_cast<Interactable*>(object.get());

				if (!interactable || !interactable->CanInteract())
				{
					continue;
				}

				const float distance = glm::distance(GetTransform().GetPosition(),
					object->GetTransform().GetPosition());

				if (distance <= closestDistance)
				{
					closestDistance = distance;
					closestInteractable = interactable;
				}
			}

			if (closestInteractable)
			{
				closestInteractable->Interact(*this);
			}
		}

		m_wasInteractDown = interactDown;
	}

	void Player::DrawUI()
	{
		MiraEngine::UI::BeginWindow("Player");
		const float healthPercent = static_cast<float>(GetHealth()) / static_cast<float>(GetMaxHealth());
		const std::string healthText = std::to_string(GetHealth()) + " / " + std::to_string(GetMaxHealth());
		MiraEngine::UI::DrawText("HP:");
		MiraEngine::UI::DrawProgressBar(healthPercent, healthText);
		MiraEngine::UI::DrawText("Equipped:");

		if (m_equippedWeapon.has_value())
		{
			MiraEngine::UI::DrawText(m_equippedWeapon->name + " (Damage: " + std::to_string(m_equippedWeapon->damage) +
				")");
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

		std::optional<std::size_t> itemToUse;
		const auto& items = m_inventory.GetItems();

		for (std::size_t i = 0; i < items.size(); ++i)
		{
			const ItemData& item = items[i];
			MiraEngine::UI::PushID(static_cast<int>(i));
			MiraEngine::UI::DrawText("- " + item.name);

			if (item.type == ItemType::Weapon && MiraEngine::UI::DrawButton("Equip"))
			{
				EquipItem(i);
			}
			if (item.type == ItemType::Consumable && MiraEngine::UI::DrawButton("Use"))
			{
				itemToUse = i;
			}

			MiraEngine::UI::PopID();
		}

		if (itemToUse)
		{
			UseItem(*itemToUse);
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

			MiraEngine::UI::DrawText(quest.name + " [" + status + "]");
			MiraEngine::UI::DrawText(quest.description);
		}

		MiraEngine::UI::EndWindow();
	}

	void Player::EquipItem(std::size_t index)
	{
		const auto& items = m_inventory.GetItems();

		if (index >= items.size())
		{
			return;
		}

		const ItemData& item = items[index];

		if (item.type != ItemType::Weapon)
		{
			return;
		}

		m_equippedWeapon = item;
		std::cout << "Equipped: " << item.name << "\n";
	}

	void Player::UnequipWeapon()
	{
		if (!m_equippedWeapon.has_value())
		{
			return;
		}

		std::cout << "Unequipped: " << m_equippedWeapon->name << "\n";
		m_equippedWeapon.reset();
	}

	void Player::UseItem(std::size_t index)
	{
		const auto& items = m_inventory.GetItems();

		if (index >= items.size())
		{
			return;
		}

		const ItemData item = items[index];

		if (item.type != ItemType::Consumable)
		{
			return;
		}

		Heal(item.healAmount);
		m_inventory.RemoveItem(index);
		std::cout << "Used: " << item.name << "\n";
	}

	void Player::AddItem(const ItemData& item)
	{
		m_inventory.AddItem(item);
		std::cout << "Picked up: " << item.name << "\n";
		m_questSystem.OnItemAdded(item.name);
	}

	bool Player::HasItem(const std::string& itemName) const
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

	bool Player::RemoveItem(const std::string& itemName)
	{
		return m_inventory.RemoveItem(itemName);
	}

	bool Player::StartQuest(const std::string& questName)
	{
		if (!m_questSystem.StartQuest(questName))
		{
			return false;
		}

		const Quest* quest = m_questSystem.GetQuest(questName);

		if (quest->requiredItem.empty())
		{
			return true;
		}

		// Already carrying what the quest asks for.
		if (HasItem(quest->requiredItem))
		{
			m_questSystem.OnItemAdded(quest->requiredItem);
		}

		return true;
	}

	const Quest* Player::GetQuest(const std::string& questName) const
	{
		return m_questSystem.GetQuest(questName);
	}

	bool Player::TurnInQuest(const std::string& questName)
	{
		return m_questSystem.TurnInQuest(questName);
	}
}