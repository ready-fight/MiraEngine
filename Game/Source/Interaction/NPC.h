#pragma once

#include "Scene/GameObject.h"
#include "Interaction/Interactable.h"

#include <string>

namespace MiraGame
{
    class NPC :
        public MiraEngine::GameObject,
        public Interactable
    {
    public:
        NPC(
            const std::string& name,
            const std::string& dialogue
        );

        bool CanInteract() const override;

        void Interact(
            Player& player
        ) override;

        void DrawUI() override;

    private:
        std::string m_dialogue;
        bool m_showDialogue = false;
    };
}