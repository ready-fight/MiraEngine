#include "Interaction/NPC.h"

#include "Player/Player.h"
#include "UI/UI.h"

#include "Assets/AssetManager.h"
#include "Animation/Animator.h"

namespace MiraGame
{
    NPC::NPC(
        const std::string& name,
        const std::string& dialogue
    )
        : MiraEngine::GameObject(name),
        m_dialogue(dialogue)
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
        m_showDialogue = true;
    }

    void NPC::DrawUI()
    {
        if (!m_showDialogue)
        {
            return;
        }

        MiraEngine::UI::BeginWindow(
            GetName()
        );

        MiraEngine::UI::DrawText(
            m_dialogue
        );

        if (
            MiraEngine::UI::DrawButton(
                "Close"
            )
            )
        {
            m_showDialogue = false;
        }

        MiraEngine::UI::EndWindow();
    }
}