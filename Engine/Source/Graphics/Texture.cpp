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
        int channelsInFile;
        // Always ask for RGBA. stb converts grayscale and RGB for us, and 4-byte pixels
        // keep every row aligned for OpenGL.
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channelsInFile, STBI_rgb_alpha);

        if (!data)
        {
            throw std::runtime_error("Failed to load texture: " + filePath);
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (GLEW_EXT_texture_filter_anisotropic)
        {
            GLfloat maxAnisotropy;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

            glTexParameterf(
                GL_TEXTURE_2D,
                GL_TEXTURE_MAX_ANISOTROPY_EXT,
                maxAnisotropy
            );
        }
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