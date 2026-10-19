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
        bool AddItem(const ItemData& item);

        void RemoveItem(std::size_t index);
        bool RemoveItem(const std::string& itemName);
        std::size_t GetCapacity() const { return m_capacity; }
        const std::vector<ItemData>& GetItems() const;

    private:
        std::size_t m_capacity = 20;
        std::vector<ItemData> m_items;
    };
}