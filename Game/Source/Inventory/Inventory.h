#pragma once

#include <string>
#include <vector>

namespace MiraGame
{
    class Inventory
    {
    public:
        void AddItem(const std::string& itemName);

        const std::vector<std::string>& GetItems() const;

    private:
        std::vector<std::string> m_items;
    };
}