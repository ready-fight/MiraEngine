#include "Quest/QuestSystem.h"

namespace MiraGame
{
    bool QuestSystem::StartQuest(
        const std::string& name
    )
    {
        // Don't add the same quest twice.
        for (const Quest& quest : m_quests)
        {
            if (quest.name == name)
            {
                return false;
            }
        }

        const Quest* questDefinition =
            QuestDatabase::GetQuest(name);

        if (!questDefinition)
        {
            return false;
        }

        Quest quest =
            *questDefinition;

        quest.status =
            QuestStatus::Active;

        m_quests.push_back(
            quest
        );

        return true;
    }

    bool QuestSystem::CompleteQuest(
        const std::string& name
    )
    {
        for (Quest& quest : m_quests)
        {
            if (
                quest.name == name &&
                quest.status == QuestStatus::Active
                )
            {
                quest.status =
                    QuestStatus::Completed;

                return true;
            }
        }

        return false;
    }

    const std::vector<Quest>&
        QuestSystem::GetQuests() const
    {
        return m_quests;
    }

    const Quest* QuestSystem::GetQuest(
        const std::string& name
    ) const
    {
        for (const Quest& quest : m_quests)
        {
            if (quest.name == name)
            {
                return &quest;
            }
        }

        return nullptr;
    }

    void QuestSystem::OnItemAdded(
        const std::string& itemName
    )
    {
        for (Quest& quest : m_quests)
        {
            if (
                quest.status == QuestStatus::Active &&
                !quest.requiredItem.empty() &&
                quest.requiredItem == itemName
                )
            {
                quest.status =
                    QuestStatus::Completed;
            }
        }
    }

    bool QuestSystem::TurnInQuest(
        const std::string& name
    )
    {
        for (Quest& quest : m_quests)
        {
            if (
                quest.name == name &&
                quest.status == QuestStatus::Completed
                )
            {
                quest.status =
                    QuestStatus::TurnedIn;

                return true;
            }
        }

        return false;
    }
}