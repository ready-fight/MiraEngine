#pragma once

#include "Quest/QuestDatabase.h"
#include "Quest/Quest.h"

#include <string>
#include <vector>

namespace MiraGame
{
    class QuestSystem
    {
    public:
        bool StartQuest(
            const std::string& name
        );

        bool CompleteQuest(
            const std::string& name
        );

        const std::vector<Quest>&
            GetQuests() const;

        const Quest* GetQuest(
            const std::string& name
        ) const;

    private:
        std::vector<Quest> m_quests;
    };
}