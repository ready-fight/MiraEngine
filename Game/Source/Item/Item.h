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

        void Pickup();
        void Update(float deltaTime);

        bool IsPickedUp() const;

        const ItemData& GetItemData() const;

        bool CanInteract() const override;

        void Interact(
            Player& player
        ) override;

    private:
        bool m_pickedUp = false;
        ItemData m_itemData;
    };
}