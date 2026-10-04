#include "Graphics/Texture.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

namespace MiraEngine
{
    Texture::Texture(const std::string& filePath)
    {
        int width;
        int height;
        int channels;

        unsigned char* data =
            stbi_load(
                filePath.c_str(),
                &width,
                &height,
                &channels,
                0
            );

        if (!data)
        {
            throw std::runtime_error(
                "Failed to load texture: " + filePath
            );
        }

        GLenum format =
            channels == 4 ? GL_RGBA : GL_RGB;

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_REPEAT
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_REPEAT
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_textureID);
    }

    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }
}