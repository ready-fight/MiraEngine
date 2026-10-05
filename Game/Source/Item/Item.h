#pragma once

#include "Scene/GameObject.h"
#include "Item/ItemData.h"

#include <string>

namespace MiraGame
{
    class Item : public MiraEngine::GameObject
    {
    public:
        explicit Item(const ItemData& itemData);

        void Pickup();

        bool IsPickedUp() const;

        const ItemData& GetItemData() const;

    private:
        bool m_pickedUp = false;
        ItemData m_itemData;
    };
}