#pragma once

#include "Scene/GameObject.h"
#include <string>

namespace MiraGame
{
    class Item : public MiraEngine::GameObject
    {
    public:
        explicit Item(const std::string& itemName);

        void Pickup();

        bool IsPickedUp() const;

        const std::string& GetItemName() const;

    private:
        bool m_pickedUp = false;
        std::string m_itemName;
    };
}