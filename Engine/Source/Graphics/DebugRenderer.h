#pragma once

#include "Graphics/Shader.h"

#include <glm/vec3.hpp>

#include <vector>

namespace MiraEngine
{
    class Camera;
    class Scene;

    class DebugRenderer
    {
    public:
        DebugRenderer();
        ~DebugRenderer();

        DebugRenderer(
            const DebugRenderer&
        ) = delete;

        DebugRenderer& operator=(
            const DebugRenderer&
            ) = delete;

        void Render(
            float aspectRatio,
            const Camera& camera,
            const Scene& scene
        );

    private:
        void AddBox(
            std::vector<glm::vec3>& vertices,
            const glm::vec3& position,
            const glm::vec3& halfExtents
        );

        void AddSphere(
            std::vector<glm::vec3>& vertices,
            const glm::vec3& position,
            float radius
        );

        Shader m_shader;

        unsigned int m_vao = 0;
        unsigned int m_vbo = 0;
    };
}