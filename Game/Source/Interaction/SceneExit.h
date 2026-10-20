#pragma once

#include "Scene/GameObject.h"

#include <glm/vec3.hpp>

#include <functional>
#include <string>
#include <utility>

namespace MiraGame
{
    class Player;

    // An invisible area that loads another scene when the player walks into it.
    class SceneExit : public MiraEngine::GameObject
    {
    public:
        SceneExit(MiraEngine::Scene& scene, Player& player, const std::string& targetScene,
            const glm::vec3& halfExtents);
        void Update(float deltaTime) override;

        // Optional: the exit only works while this returns true, e.g. "the door is open".
        void SetCondition(std::function<bool()> condition) { m_condition = std::move(condition); }

    private:
        MiraEngine::Scene& m_scene;
        Player& m_player;
        std::string m_targetScene;
        std::function<bool()> m_condition;
        bool m_triggered = false;
    };
}