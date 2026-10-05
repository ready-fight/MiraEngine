#pragma once

#include <string>

namespace MiraGame
{
    enum class ItemType
    {
        Weapon,
        Consumable,
        KeyItem
    };

    struct ItemData
    {
        std::string name;
        ItemType type = ItemType::Consumable;
        
        int damage = 0;
        int healAmount = 0;
    };
}