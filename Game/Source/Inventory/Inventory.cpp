#include "Inventory/Inventory.h"

namespace MiraGame
{
    void Inventory::AddItem(
        const std::string& itemName
    )
    {
        m_items.push_back(itemName);
    }

    const std::vector<std::string>&
        Inventory::GetItems() const
    {
        return m_items;
    }
}