#pragma once

#include "Quest/Quest.h"

#include <string>
#include <unordered_map>

namespace MiraGame
{
    class QuestDatabase
    {
    public:
        static void RegisterQuest(
            const Quest& quest
        );

        static const Quest* GetQuest(
            const std::string& name
        );

    private:
        static std::unordered_map<
            std::string,
            Quest
        > s_quests;
    };
}