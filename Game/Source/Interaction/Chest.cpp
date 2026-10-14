#include "Interaction/Chest.h"
#include "Player/Player.h"

#include <iostream>

namespace MiraGame
{
    Chest::Chest(const ItemData& contents)
        : MiraEngine::GameObject("Chest")
        , m_contents(contents)
    {
        SetCollider(MiraEngine::Collider::Sphere(0.5f));
    }

    void Chest::Update(float deltaTime)
    {
        if (m_isOpen)
        {
            FadeOut(deltaTime);
        }
    }

    bool Chest::CanInteract() const
    {
        return !m_isOpen;
    }

    void Chest::Interact(Player& player)
    {
        if (m_isOpen)
        {
            return;
        }

        player.AddItem(m_contents);
        m_isOpen = true;
        std::cout << "Chest opened: received " << m_contents.name << "\n";
    }
}