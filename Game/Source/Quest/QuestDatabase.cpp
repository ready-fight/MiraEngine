#include "Quest/QuestDatabase.h"

namespace MiraGame
{
    std::unordered_map<std::string, Quest>
        QuestDatabase::s_quests;

    void QuestDatabase::RegisterQuest(
        const Quest& quest
    )
    {
        s_quests[quest.name] = quest;
    }

    const Quest* QuestDatabase::GetQuest(
        const std::string& name
    )
    {
        const auto it =
            s_quests.find(name);

        if (it == s_quests.end())
        {
            return nullptr;
        }

        return &it->second;
    }
}