#include "Interaction/SceneExit.h"
#include "Player/Player.h"

#include "Collision/Collider.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace MiraGame
{
    SceneExit::SceneExit(MiraEngine::Scene& scene, Player& player, const std::string& targetScene,
        const glm::vec3& halfExtents)
        : MiraEngine::GameObject("SceneExit")
        , m_scene(scene)
        , m_player(player)
        , m_targetScene(targetScene)
    {
        MiraEngine::Collider area = MiraEngine::Collider::Box(halfExtents);
        area.isTrigger = true;
        SetCollider(area);
    }

    void SceneExit::Update(float /*deltaTime*/)
    {
        if (m_triggered || m_player.IsDead())
        {
            return;
        }

        if (m_condition && !m_condition())
        {
            return;
        }

        if (m_scene.Overlaps(*this, m_player))
        {
            // Request once; the scene actually changes at the start of the next frame.
            m_triggered = true;
            MiraEngine::SceneManager::RequestChange(m_targetScene);
        }
    }
}