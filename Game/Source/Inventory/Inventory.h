#pragma once

#include "Item/ItemData.h"

#include <string>
#include <vector>
#include <cstddef>

namespace MiraGame
{
    class Inventory
    {
    public:
        void AddItem(const ItemData& item);
        void RemoveItem(std::size_t index);
        bool RemoveItem(
            const std::string& itemName
        );

        const std::vector<ItemData>& GetItems() const;

    private:
        std::vector<ItemData> m_items;
    };
}