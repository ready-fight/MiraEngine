#pragma once

#include <string>

namespace MiraGame
{
    enum class QuestStatus
    {
        Inactive,
        Active,
        Completed,
        TurnedIn
    };

    struct Quest
    {
        std::string name;
        std::string description;

        std::string requiredItem;

        QuestStatus status =
            QuestStatus::Inactive;
    };
}