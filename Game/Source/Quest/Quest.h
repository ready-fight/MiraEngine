#pragma once

#include <string>

namespace MiraGame
{
    enum class QuestStatus
    {
        Inactive,
        Active,
        Completed
    };

    struct Quest
    {
        std::string name;
        std::string description;

        QuestStatus status =
            QuestStatus::Inactive;
    };
}