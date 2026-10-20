#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace MiraEngine
{
    class Scene;

    class SceneManager
    {
    public:
        // A builder fills an empty scene with its objects.
        using SceneBuilder = std::function<void(Scene&)>;

        SceneManager() = delete;
        static void Register(const std::string& name, SceneBuilder builder);

        // Safe to call from anywhere, including object Updates: the change happens at the start of the next frame.
        static void RequestChange(const std::string& name);

        // Called by Application once per frame. Returns true if the scene was rebuilt.
        static bool ApplyPendingChange(Scene& scene);

        static const std::string& GetCurrentName() { return s_currentName; }

    private:
        static std::unordered_map<std::string, SceneBuilder> s_builders;
        static std::string s_pendingName;
        static std::string s_currentName;
    };
}