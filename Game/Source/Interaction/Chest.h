#pragma once

#include "Scene/GameObject.h"
#include "Interaction/Interactable.h"
#include "Item/ItemData.h"

namespace MiraGame
{
    class Chest :
        public MiraEngine::GameObject,
        public Interactable
    {
    public:
        explicit Chest(const ItemData& itemData);

        bool CanInteract() const override;

        void Interact(
            Player& player
        ) override;

    private:
        bool m_isOpen = false;
        ItemData m_itemData;
    };
}