#pragma once

#include "Character/Character.h"
#include "Inventory/Inventory.h"

#include <optional>
#include <cmath>
#include <cstddef>
#include <string>

namespace MiraEngine
{
    class Scene;
}

namespace MiraGame
{
    class Player : public Character
    {
    public:
        explicit Player(MiraEngine::Scene* scene);

        void Update(float deltaTime) override;
        void DrawUI() override;
        void EquipItem(std::size_t index);
        void UnequipWeapon();
        void UseItem(std::size_t index);
        void AddItem(
            const ItemData& item
        );
        bool HasItem(
            const std::string& itemName
        ) const;
        bool RemoveItem(
            const std::string& itemName
        );

    private:
        float m_speed = 4.0f;
        bool m_isMoving = false;

        float m_attackOffset = 1.0f;
        float m_attackRadius = 0.8f;
        Inventory m_inventory;

        bool m_wasAttackDown = false;
        bool m_isAttacking = false;
        bool m_attackHitApplied = false;
        bool m_hasDied = false;

        float m_attackTime = 0.0f;
        float m_attackHitTime = 0.55f;
        float m_attackDuration = 0.6f;

        float m_interactRange = 1.5f;
        bool m_wasInteractDown = false;

        MiraEngine::Scene* m_scene = nullptr;

        std::optional<ItemData> m_equippedWeapon;
    };
}