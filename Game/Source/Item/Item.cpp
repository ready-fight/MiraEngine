#include "Item/Item.h"

namespace MiraGame
{
    Item::Item()
        : MiraEngine::GameObject("Item")
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
}