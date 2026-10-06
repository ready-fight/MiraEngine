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

    bool Inventory::RemoveItem(
        const std::string& itemName
    )
    {
        for (auto it = m_items.begin();
            it != m_items.end();
            ++it)
        {
            if (it->name == itemName)
            {
                m_items.erase(it);
                return true;
            }
        }

        return false;
    }

    const std::vector<ItemData>&
        Inventory::GetItems() const
    {
        return m_items;
    }
}