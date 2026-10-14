#pragma once

#include "Scene/GameObject.h"
#include "Interaction/Interactable.h"

#include <cstddef>
#include <string>
#include <vector>

namespace MiraGame
{
    // What an NPC says, depending on where the player is in that NPC's quest.
    struct Dialogue
    {
        std::vector<std::string> beforeQuest;
        std::vector<std::string> questActive;
        std::vector<std::string> questCompleted;
        std::vector<std::string> questTurnedIn;
    };

    class NPC : public MiraEngine::GameObject, public Interactable
    {
    public:
        NPC(const std::string& name, const Dialogue& dialogue, const std::string& quest = "");

        bool CanInteract() const override;
        void Interact(Player& player) override;
        void DrawUI() override;

    private:
        void FinishConversation();

        Dialogue m_dialogue;
        std::string m_quest;

        Player* m_talkingTo = nullptr;
        const std::vector<std::string>* m_lines = nullptr;
        std::size_t m_lineIndex = 0;
    };
}