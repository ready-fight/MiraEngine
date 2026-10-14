#include "Interaction/NPC.h"
#include "Collision/Collider.h"

#include "Player/Player.h"
#include "UI/UI.h"

#include "Assets/AssetManager.h"

namespace MiraGame
{
    NPC::NPC(const std::string& name, const Dialogue& dialogue, const std::string& quest)
        : MiraEngine::GameObject(name)
        , m_dialogue(dialogue)
        , m_quest(quest)
    {
        auto model = MiraEngine::AssetManager::LoadModel("Assets/Models/Idle.fbx");
        SetModel(model);
        SetCollider(MiraEngine::Collider::Sphere(0.3f));
        GetTransform().SetScale(glm::vec3(0.01f));

        PlayAnimation("NPC");
    }

    bool NPC::CanInteract() const
    {
        return true;
    }

    void NPC::Interact(Player& player)
    {
        const Quest* quest = m_quest.empty() ? nullptr : player.GetQuest(m_quest);
        const QuestStatus status = quest ? quest->status : QuestStatus::Inactive;

        switch (status)
        {
            case QuestStatus::Inactive:  m_lines = &m_dialogue.beforeQuest; break;
            case QuestStatus::Active:    m_lines = &m_dialogue.questActive; break;
            case QuestStatus::Completed: m_lines = &m_dialogue.questCompleted; break;
            case QuestStatus::TurnedIn:  m_lines = &m_dialogue.questTurnedIn; break;
        }

        if (!m_lines->empty())
        {
            m_talkingTo = &player;
            m_lineIndex = 0;
        }
    }

    void NPC::DrawUI()
    {
        if (!m_talkingTo)
        {
            return;
        }

        MiraEngine::UI::BeginWindow(GetName());
        MiraEngine::UI::DrawText((*m_lines)[m_lineIndex]);

        const bool onLastLine = m_lineIndex + 1 == m_lines->size();

        if (!onLastLine)
        {
            if (MiraEngine::UI::DrawButton("Next"))
            {
                ++m_lineIndex;
            }
        }
        else if (MiraEngine::UI::DrawButton("Close"))
        {
            FinishConversation();
        }

        MiraEngine::UI::EndWindow();
    }

    // Closing the conversation is what moves the quest along.
    void NPC::FinishConversation()
    {
        if (!m_quest.empty())
        {
            const Quest* quest = m_talkingTo->GetQuest(m_quest);

            if (!quest)
            {
                m_talkingTo->StartQuest(m_quest);
            }
            else if (quest->status == QuestStatus::Completed)
            {
                m_talkingTo->TurnInQuest(m_quest);
            }
        }

        m_talkingTo = nullptr;
    }
}