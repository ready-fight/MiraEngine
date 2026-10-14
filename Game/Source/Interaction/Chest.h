#pragma once

#include "Scene/GameObject.h"
#include "Interaction/Interactable.h"
#include "Item/ItemData.h"

namespace MiraGame
{
    class Chest : public MiraEngine::GameObject, public Interactable
    {
    public:
        explicit Chest(const ItemData& itemData);

        void Update(float deltaTime) override;
        bool CanInteract() const override;
        void Interact(Player& player) override;

    private:
        ItemData m_contents;
        bool m_isOpen = false;
    };
}