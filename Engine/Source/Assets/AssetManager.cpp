#include "Assets/AssetManager.h"

#include "Graphics/Model.h"

namespace Mira
{
    std::unordered_map<std::string, std::shared_ptr<Model>> AssetManager::s_models;

    std::shared_ptr<Model> AssetManager::LoadModel(
        const std::string& path
    )
    {
        const auto it = s_models.find(path);

        if (it != s_models.end())
        {
            return it->second;
        }

        auto model = std::make_shared<Model>(path);

        s_models[path] = model;

        return model;
    }

    void AssetManager::Clear()
    {
        s_models.clear();
    }
}