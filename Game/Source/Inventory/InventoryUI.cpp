#include "Inventory/InventoryUI.h"

#include "Player/Player.h"
#include "Core/Input.h"
#include "UI/UI.h"

#include <cstddef>
#include <optional>
#include <string>

namespace MiraGame
{
    void InventoryUI::Update()
    {
        const bool toggleDown = MiraEngine::Input::IsKeyPressed(MiraEngine::Key::I);

        if (toggleDown && !m_wasToggleDown)
        {
            m_isOpen = !m_isOpen;
        }

        m_wasToggleDown = toggleDown;
    }

    void InventoryUI::Draw(Player& player)
    {
        if (!m_isOpen)
        {
            return;
        }

        // The selected item may have been removed since last frame (used, or handed in for a quest).
        if (m_selectedSlot && *m_selectedSlot >= player.GetInventory().GetItems().size())
        {
            m_selectedSlot.reset();
        }

        if (MiraEngine::UI::BeginWindow("Inventory", &m_isOpen, true))
        {
            const auto& weapon = player.GetEquippedWeapon();
            MiraEngine::UI::DrawText("Equipped: " + (weapon ? weapon->name : std::string("None")));

            if (weapon && MiraEngine::UI::DrawButton("Unequip"))
            {
                player.UnequipWeapon();
            }

            MiraEngine::UI::DrawSeparator();
            DrawGrid(player.GetInventory());
            MiraEngine::UI::DrawSeparator();
            DrawSelectedItem(player);
        }

        MiraEngine::UI::EndWindow();
    }

    void InventoryUI::DrawGrid(const Inventory& inventory)
    {
        const auto& items = inventory.GetItems();

        for (std::size_t slot = 0; slot < inventory.GetCapacity(); ++slot)
        {
            // Stay on the same row until it has Columns slots.
            if (slot % Columns != 0)
            {
                MiraEngine::UI::SameLine();
            }

            const bool hasItem = slot < items.size();
            const std::string label = hasItem ? items[slot].name : "";

            MiraEngine::UI::PushID(static_cast<int>(slot));

            if (MiraEngine::UI::DrawSlot(label, SlotSize, m_selectedSlot == slot))
            {
                // Clicking an empty slot clears the selection.
                if (hasItem)
                {
                    m_selectedSlot = slot;
                }
                else
                {
                    m_selectedSlot.reset();
                }
            }

            MiraEngine::UI::PopID();
        }
    }

    void InventoryUI::DrawSelectedItem(Player& player)
    {
        if (!m_selectedSlot)
        {
            MiraEngine::UI::DrawText("Select an item.");
            return;
        }

        const std::size_t slot = *m_selectedSlot;
        const ItemData& item = player.GetInventory().GetItems()[slot];
        MiraEngine::UI::DrawText(item.name);

        switch (item.type)
        {
        case ItemType::Weapon:
            MiraEngine::UI::DrawText("Damage: " + std::to_string(item.damage));

            if (MiraEngine::UI::DrawButton("Equip"))
            {
                player.EquipItem(slot);
            }
            break;

        case ItemType::Consumable:
            MiraEngine::UI::DrawText("Heals: " + std::to_string(item.healAmount));

            if (MiraEngine::UI::DrawButton("Use"))
            {
                // Using it removes it from the inventory, so `item` is invalid after this line.
                player.UseItem(slot);
                m_selectedSlot.reset();
            }
            break;

        case ItemType::KeyItem:
            MiraEngine::UI::DrawText("Key item");
            break;
        }
    }
}