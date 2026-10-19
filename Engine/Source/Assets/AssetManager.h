#pragma once

#include "Graphics/Texture.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace MiraEngine
{
    class Model;
    struct AnimationClip;

    class AssetManager
    {
    public:
        static std::shared_ptr<Model> LoadModel(const std::string& path);
        static const std::vector<std::shared_ptr<AnimationClip>>& LoadAnimations(const std::string& path);
        static std::shared_ptr<Texture> LoadTexture(const std::string& path);
        static void Clear();

    private:
        static std::unordered_map<std::string, std::shared_ptr<Model>> s_models;
        static std::unordered_map<std::string, std::vector<std::shared_ptr<AnimationClip>>> s_animations;
        static std::unordered_map<std::string, std::shared_ptr<Texture>> s_textures;
    };
}