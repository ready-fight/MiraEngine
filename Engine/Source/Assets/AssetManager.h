#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace MiraEngine
{
    class Model;
    struct AnimationClip;

    class AssetManager
    {
    public:

        static std::shared_ptr<Model> LoadModel(
            const std::string& path
        );

        static std::shared_ptr<AnimationClip> LoadAnimation(
            const std::string& path
        );

        static std::shared_ptr<AnimationClip> GetAnimation(const std::string& name);

        static void Clear();

    private:

        static std::unordered_map<
            std::string,
            std::shared_ptr<Model>
        > s_models;

        static std::unordered_map<
            std::string,
            std::shared_ptr<AnimationClip>
        > s_animations;
    };
}