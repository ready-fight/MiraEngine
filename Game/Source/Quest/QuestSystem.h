#pragma once

#include "Quest/Quest.h"

#include <string>
#include <vector>

namespace MiraGame
{
    // A player's progress through quests. Definitions live in QuestDatabase.
    class QuestSystem
    {
    public:
        bool StartQuest(const std::string& name);
        bool TurnInQuest(const std::string& name);
        void OnItemAdded(const std::string& itemName);

        const Quest* GetQuest(const std::string& name) const;
        const std::vector<Quest>& GetQuests() const;

    private:
        Quest* Find(const std::string& name);

        std::vector<Quest> m_quests;
    };
}