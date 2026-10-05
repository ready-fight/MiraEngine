#include "Inventory/Inventory.h"

namespace MiraGame
{
    void Inventory::AddItem(
        const ItemData& item
    )
    {
        m_items.push_back(item);
    }

    const std::vector<ItemData>&
        Inventory::GetItems() const
    {
        return m_items;
    }
}