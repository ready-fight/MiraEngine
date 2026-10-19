#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Light.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

namespace MiraEngine
{
    class Camera;
    class Scene;
    class ParticleEmitter;

    class ParticleRenderer
    {
    public:
        ParticleRenderer();
        ~ParticleRenderer();
        ParticleRenderer(const ParticleRenderer&) = delete;
        ParticleRenderer& operator=(const ParticleRenderer&) = delete;

        void Render(float aspectRatio, const Camera& camera, const Scene& scene, const Fog& fog);

    private:
        void DrawEmitter(const ParticleEmitter& emitter);

        // What the GPU gets per particle; must match the attribute setup in the constructor.
        struct Instance
        {
            glm::vec3 position;
            float size;
            glm::vec4 color;
        };

        Shader m_shader;
        unsigned int m_vao = 0;
        unsigned int m_quadVbo = 0;
        unsigned int m_instanceVbo = 0;
        std::vector<Instance> m_instances;
    };
}