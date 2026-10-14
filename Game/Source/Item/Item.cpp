#include "Item/Item.h"
#include "Player/Player.h"

namespace MiraGame
{
    Item::Item(const ItemData& itemData)
        : MiraEngine::GameObject(itemData.name)
        , m_itemData(itemData)
    {
    }

    void Item::Update(float deltaTime)
    {
        if (m_pickedUp)
        {
            FadeOut(deltaTime);
        }
    }

    bool Item::IsPickedUp() const
    {
        return m_pickedUp;
    }

    bool Item::CanInteract() const
    {
        return !m_pickedUp;
    }

    void Item::Interact(Player& player)
    {
        if (m_pickedUp)
        {
            return;
        }

        player.AddItem(m_itemData);
        m_pickedUp = true;
    }
}