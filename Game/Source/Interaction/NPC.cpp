#include "Interaction/NPC.h"

#include "Player/Player.h"
#include "UI/UI.h"

#include "Assets/AssetManager.h"
#include "Animation/Animator.h"

namespace MiraGame
{
    NPC::NPC(
        const std::string& name,
        const std::vector<std::string>& dialogue,
        const std::string& questToStart
    )
        : MiraEngine::GameObject(name),
        m_dialogue(dialogue),
        m_questToStart(questToStart)
    {
        auto model = MiraEngine::AssetManager::LoadModel(
            "Assets/Models/NPC.fbx"
        );

        SetModel(model);

        GetTransform().SetScale(
            glm::vec3(0.01f)
        );

        if (MiraEngine::Animator* animator = GetAnimator())
        {
            animator->PlayAnimation(0);
        }
    }

    bool NPC::CanInteract() const
    {
        return true;
    }

    void NPC::Interact(
        Player& player
    )
    {
        m_interactingPlayer =
            &player;

        m_dialogueIndex = 0;
        m_showDialogue = true;
    }

    void NPC::DrawUI()
    {
        if (
            !m_showDialogue ||
            m_dialogue.empty()
            )
        {
            return;
        }

        MiraEngine::UI::BeginWindow(
            GetName()
        );

        MiraEngine::UI::DrawText(
            m_dialogue[m_dialogueIndex]
        );

        if (
            m_dialogueIndex + 1 <
            m_dialogue.size()
            )
        {
            if (
                MiraEngine::UI::DrawButton(
                    "Next"
                )
                )
            {
                ++m_dialogueIndex;
            }
        }
        else
        {
            if (
                MiraEngine::UI::DrawButton(
                    "Close"
                )
                )
            {
                if (
                    m_interactingPlayer &&
                    !m_questToStart.empty()
                    )
                {
                    m_interactingPlayer->StartQuest(
                        m_questToStart
                    );
                }

                m_showDialogue = false;
                m_interactingPlayer = nullptr;
            }
        }

        MiraEngine::UI::EndWindow();
    }
}