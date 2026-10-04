#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace MiraEngine
{
    class Model;

    class AssetManager
    {
    public:
        static std::shared_ptr<Model> LoadModel(
            const std::string& path
        );

        static void Clear();

    private:
        static std::unordered_map<
            std::string,
            std::shared_ptr<Model>
        > s_models;
    };
}