#pragma once

#include "Scene/GameObject.h"
#include "Interaction/Interactable.h"

#include <string>
#include <vector>

namespace MiraGame
{
    class NPC :
        public MiraEngine::GameObject,
        public Interactable
    {
    public:
        NPC(
            const std::string& name,
            const std::vector<std::string>& dialogue,
            const std::string& questToStart = ""
        );

        bool CanInteract() const override;

        void Interact(
            Player& player
        ) override;

        void DrawUI() override;

    private:
        std::vector<std::string> m_dialogue;
        std::size_t m_dialogueIndex = 0;

        std::string m_questToStart;

        bool m_showDialogue = false;

        Player* m_interactingPlayer =
            nullptr;
    };
}