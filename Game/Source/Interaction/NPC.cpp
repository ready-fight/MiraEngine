#include "Interaction/NPC.h"

#include "Player/Player.h"
#include "UI/UI.h"

#include "Assets/AssetManager.h"
#include "Animation/Animator.h"

namespace MiraGame
{
    NPC::NPC(
        const std::string& name,
        const std::vector<std::string>& inactiveDialogue,
        const std::vector<std::string>& activeDialogue,
        const std::vector<std::string>& completedDialogue,
        const std::vector<std::string>& turnedInDialogue,
        const std::string& questToStart
    )
        : MiraEngine::GameObject(name),
        m_inactiveDialogue(inactiveDialogue),
        m_activeDialogue(activeDialogue),
        m_completedDialogue(completedDialogue),
        m_turnedInDialogue(turnedInDialogue),
        m_questToStart(questToStart)
    {
        auto model = MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Interactable/NPC.fbx"
        );

        SetModel(model);

        SetCollider(
            std::make_unique<MiraEngine::SphereCollider>(
                0.3f
            )
        );

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
        m_interactingPlayer = &player;
        m_dialogueIndex = 0;

        const Quest* quest = nullptr;

        if (!m_questToStart.empty())
        {
            quest = player.GetQuest(
                m_questToStart
            );
        }

        if (!quest)
        {
            m_dialogue = m_inactiveDialogue;
        }
        else
        {
            switch (quest->status)
            {
            case QuestStatus::Active:
                m_dialogue = m_activeDialogue;
                break;

            case QuestStatus::Completed:
                m_dialogue = m_completedDialogue;
                break;

            case QuestStatus::TurnedIn:
                m_dialogue = m_turnedInDialogue;
                break;

            default:
                m_dialogue = m_inactiveDialogue;
                break;
            }
        }

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
                    const Quest* quest =
                        m_interactingPlayer->GetQuest(
                            m_questToStart
                        );

                    if (!quest)
                    {
                        m_interactingPlayer->StartQuest(
                            m_questToStart
                        );
                    }
                    else if (
                        quest->status ==
                        QuestStatus::Completed
                        )
                    {
                        m_interactingPlayer->TurnInQuest(
                            m_questToStart
                        );
                    }
                }

                m_showDialogue = false;
                m_interactingPlayer = nullptr;
            }
        }

        MiraEngine::UI::EndWindow();
    }
}