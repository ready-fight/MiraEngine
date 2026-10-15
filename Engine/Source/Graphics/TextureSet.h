#pragma once

#include <memory>

namespace MiraEngine
{
    class Texture;

    struct TextureSet
    {
        std::shared_ptr<Texture> color;
        std::shared_ptr<Texture> normal;
        std::shared_ptr<Texture> occlusion;
        std::shared_ptr<Texture> roughness;
    };

    enum class TextureSlot
    {
        Color,
        Normal,
        Occlusion,
        Roughness,
    };
}