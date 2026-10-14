#include "Quest/QuestSystem.h"
#include "Quest/QuestDatabase.h"

#include <algorithm>

namespace MiraGame
{
    bool QuestSystem::StartQuest(const std::string& name)
    {
        if (GetQuest(name))
        {
            return false; // already started
        }

        const Quest* definition = QuestDatabase::GetQuest(name);

        if (!definition)
        {
            return false;
        }

        Quest quest = *definition;
        quest.status = QuestStatus::Active;
        m_quests.push_back(quest);
        return true;
    }

    bool QuestSystem::TurnInQuest(const std::string& name)
    {
        Quest* quest = Find(name);

        if (!quest || quest->status != QuestStatus::Completed)
        {
            return false;
        }

        quest->status = QuestStatus::TurnedIn;
        return true;
    }

    void QuestSystem::OnItemAdded(const std::string& itemName)
    {
        for (Quest& quest : m_quests)
        {
            if (quest.status == QuestStatus::Active && !quest.requiredItem.empty() && quest.requiredItem == itemName)
            {
                quest.status = QuestStatus::Completed;
            }
        }
    }

    const Quest* QuestSystem::GetQuest(const std::string& name) const
    {
        const auto it = std::find_if(m_quests.begin(), m_quests.end(),
            [&](const Quest& quest) { return quest.name == name; });

        return it != m_quests.end() ? &*it : nullptr;
    }

    const std::vector<Quest>& QuestSystem::GetQuests() const
    {
        return m_quests;
    }

    Quest* QuestSystem::Find(const std::string& name)
    {
        // Safe: this object isn't const, so neither is the quest.
        return const_cast<Quest*>(GetQuest(name));
    }
}