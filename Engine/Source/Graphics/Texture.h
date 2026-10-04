#pragma once

#include <string>

namespace MiraEngine
{
    class Texture
    {
    public:
        explicit Texture(const std::string& filePath);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        void Bind(unsigned int slot = 0) const;

    private:
        unsigned int m_textureID = 0;
    };
}