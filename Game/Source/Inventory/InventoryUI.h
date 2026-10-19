#pragma once

#include "Inventory/Inventory.h"

#include <cstddef>
#include <optional>

namespace MiraGame
{
    class Player;

    class InventoryUI
    {
    public:
        void Update();
        void Draw(Player& player);

    private:
        void DrawGrid(const Inventory& inventory);
        void DrawSelectedItem(Player& player);

        static constexpr std::size_t Columns = 5;
        static constexpr float SlotSize = 64.0f;

        bool m_isOpen = false;
        bool m_wasToggleDown = false;
        std::optional<std::size_t> m_selectedSlot;
    };
}