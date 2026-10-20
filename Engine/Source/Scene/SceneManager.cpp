#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include <iostream>
#include <utility>

namespace MiraEngine
{
    std::unordered_map<std::string, SceneManager::SceneBuilder> SceneManager::s_builders;
    std::string SceneManager::s_pendingName;
    std::string SceneManager::s_currentName;

    void SceneManager::Register(const std::string& name, SceneBuilder builder)
    {
        s_builders[name] = std::move(builder);
    }

    void SceneManager::RequestChange(const std::string& name)
    {
        s_pendingName = name;
    }

    bool SceneManager::ApplyPendingChange(Scene& scene)
    {
        if (s_pendingName.empty())
        {
            return false;
        }

        // Take the name first, so a builder could request another change without it being lost.
        const std::string name = std::move(s_pendingName);
        s_pendingName.clear();

        const auto it = s_builders.find(name);

        if (it == s_builders.end())
        {
            std::cerr << "Scene not found: " << name << "\n";
            return false;
        }

        scene.Clear();
        it->second(scene);
        s_currentName = name;
        return true;
    }
}