#include "Interaction/Chest.h"
#include "Player/Player.h"
#include "Collision/SphereCollider.h"

#include <iostream>

namespace MiraGame
{

    Chest::Chest(const ItemData& itemData)
		: MiraEngine::GameObject("Chest"), m_itemData(itemData)
    {
        SetCollider(
            std::make_unique<MiraEngine::SphereCollider>(
                0.5f
            )
        );
    }

    void Chest::Update(float deltaTime) {

        if (m_isOpen) {
            FadeOut(deltaTime);
        }
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