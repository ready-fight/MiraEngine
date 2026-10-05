#include "Inventory/Inventory.h"

namespace MiraGame
{
    void Inventory::AddItem(
        const ItemData& item
    )
    {
        m_items.push_back(item);
    }

    void Inventory::RemoveItem(std::size_t index)
    {
        if (index >= m_items.size())
        {
            return;
        }

        m_items.erase(
            m_items.begin() + index
        );
    }

    const std::vector<ItemData>&
        Inventory::GetItems() const
    {
        return m_items;
    }
}