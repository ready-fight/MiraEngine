#include "Item.h"
#include "Item/Item.h"
#include "Player/Player.h"

namespace MiraGame
{
    Item::Item(const ItemData& itemData)
        : MiraEngine::GameObject(itemData.name),
        m_itemData(itemData)
    {
    }

    void Item::Update(float deltaTime) {
        if (m_pickedUp) {
            FadeOut(deltaTime);
        }
    }

    void Item::Pickup()
    {
        if (m_pickedUp)
        {
            return;
        }

        m_pickedUp = true;

        // Remove the visible model.
        //SetModel(nullptr);
    }

    bool Item::IsPickedUp() const
    {
        return m_pickedUp;
    }

    const ItemData& Item::GetItemData() const
    {
        return m_itemData;
    }

    bool Item::CanInteract() const
    {
        return !m_pickedUp;
    }

    void Item::Interact(
        Player& player
    )
    {
        if (m_pickedUp)
        {
            return;
        }

        player.AddItem(
            m_itemData
        );

        Pickup();
    }
}