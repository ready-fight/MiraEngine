#include "Item.h"
#include "Item/Item.h"

namespace MiraGame
{
    Item::Item(const std::string& itemName) : MiraEngine::GameObject("Item"), m_itemName(itemName)
    {
    }

    void Item::Pickup()
    {
        if (m_pickedUp)
        {
            return;
        }

        m_pickedUp = true;

        // Remove the visible model.
        SetModel(nullptr);
    }

    bool Item::IsPickedUp() const
    {
        return m_pickedUp;
    }

    const std::string& Item::GetItemName() const
    {
        return m_itemName;
    }
}