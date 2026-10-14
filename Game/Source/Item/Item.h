#pragma once

#include "Scene/GameObject.h"
#include "Item/ItemData.h"
#include "Interaction/Interactable.h"

#include <string>

namespace MiraGame
{
    class Item : public MiraEngine::GameObject, public Interactable
    {
    public:
        explicit Item(const ItemData& itemData);
        void Update(float deltaTime) override;
        bool IsPickedUp() const;
        bool CanInteract() const override;
        void Interact(Player& player) override;

    private:
        ItemData m_itemData;
        bool m_pickedUp = false;
    };
}