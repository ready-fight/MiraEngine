#include "Interaction/Chest.h"
#include "Player/Player.h"

#include <iostream>

namespace MiraGame
{
    Chest::Chest(const ItemData& itemData)
		: MiraEngine::GameObject("Chest"), m_itemData(itemData)
    {
    }

    bool Chest::CanInteract() const
    {
        return !m_isOpen;
    }

    void Chest::Interact(
        Player& player
    )
    {
        if (m_isOpen)
        {
            return;
        }

        player.AddItem(
            m_itemData
        );

        m_isOpen = true;

        std::cout
            << "Chest opened: received "
            << m_itemData.name
            << "\n";
    }
}