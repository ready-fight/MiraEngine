#pragma once

#include "Item/ItemData.h"

#include <string>
#include <vector>

namespace MiraGame
{
    class Inventory
    {
    public:
        void AddItem(const ItemData& item);

        const std::vector<ItemData>& GetItems() const;

    private:
        std::vector<ItemData> m_items;
    };
}