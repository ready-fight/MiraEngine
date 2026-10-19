#include "Graphics/ParticleRenderer.h"
#include "Particles/ParticleEmitter.h"
#include "Graphics/Camera.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include <GL/glew.h>

#include <glm/common.hpp>
#include <cstddef>

namespace
{
    const char* ParticleVertexShader = R"(
        #version 460 core

        layout(location = 0) in vec2 corner;          // same quad for every particle
        layout(location = 1) in vec4 positionAndSize; // per particle: xyz = position, w = size
        layout(location = 2) in vec4 color;           // per particle

        uniform mat4 view;
        uniform mat4 projection;
        uniform vec3 cameraPosition;
        uniform float fogStart;
        uniform float fogEnd;

        out vec4 particleColor;
        out vec2 localPosition;

        void main()
        {
                // The first two rows of the view matrix are the camera's right and up axes in world space.
                vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
                vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

                vec3 offset = (cameraRight * corner.x + cameraUp * corner.y) * positionAndSize.w;
                vec3 worldPosition = positionAndSize.xyz + offset;

                // Fade particles out with distance instead of tinting them: whatever is behind
                // a fogged particle is fogged too, and tinting additive particles would brighten them.
                float distanceToCamera = length(positionAndSize.xyz - cameraPosition);
                float fogAmount = clamp((distanceToCamera - fogStart) / (fogEnd - fogStart), 0.0, 1.0);

                particleColor = vec4(color.rgb, color.a * (1.0 - fogAmount));
                localPosition = corner;
                gl_Position = projection * view * vec4(worldPosition, 1.0);
        }
    )";

    const char* ParticleFragmentShader = R"(
        #version 460 core

        in vec4 particleColor;
        in vec2 localPosition; // -0.5..0.5 across the quad
        out vec4 fragmentColor;

        void main()
        {
            // Soft round particle: solid in the middle, fading to nothing at the circle's edge.
            float distanceFromCenter = length(localPosition) * 2.0; // 0 = center, 1 = edge
            float softness = clamp(1.0 - distanceFromCenter, 0.0, 1.0);

            fragmentColor = vec4(particleColor.rgb, particleColor.a * softness * softness);
        }
    )";
}

namespace MiraEngine
{
    ParticleRenderer::ParticleRenderer()
        : m_shader(ParticleVertexShader, ParticleFragmentShader)
    {
        // A unit quad as a triangle strip.
        const float corners[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
            -0.5f,  0.5f,
             0.5f,  0.5f,
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_quadVbo);
        glGenBuffers(1, &m_instanceVbo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        // Divisor 1 = advance once per particle instead of once per vertex.
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVbo);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Instance),
            reinterpret_cast<void*>(offsetof(Instance, position)));  // reads position + size
        glVertexAttribDivisor(1, 1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Instance),
            reinterpret_cast<void*>(offsetof(Instance, color)));
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ParticleRenderer::~ParticleRenderer()
    {
        glDeleteBuffers(1, &m_instanceVbo);
        glDeleteBuffers(1, &m_quadVbo);
        glDeleteVertexArrays(1, &m_vao);
    }

    void ParticleRenderer::Render(float aspectRatio, const Camera& camera, const Scene& scene, const Fog& fog)
    {
        m_shader.Bind();
        m_shader.SetMatrix4("view", camera.GetViewMatrix());
        m_shader.SetMatrix4("projection", camera.GetProjectionMatrix(aspectRatio));
        m_shader.SetVector3("cameraPosition", camera.GetPosition());
        m_shader.SetFloat("fogStart", fog.start);
        m_shader.SetFloat("fogEnd", fog.end);

        // Test against depth so walls hide particles, but don't write it,
        // or see-through particles would cut holes in the ones behind them.
        glDepthMask(GL_FALSE);
        glBindVertexArray(m_vao);

        for (const auto& object : scene.GetObjects())
        {
            const ParticleEmitter* emitter = object->GetParticleEmitter();

            if (emitter)
            {
                DrawEmitter(*emitter);
            }
        }

        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        m_shader.Unbind();
    }

    void ParticleRenderer::DrawEmitter(const ParticleEmitter& emitter)
    {
        const std::size_t count = emitter.GetAliveCount();

        if (count == 0)
        {
            return;
        }

        const ParticleSettings& settings = emitter.GetSettings();
        const auto& particles = emitter.GetParticles();

        m_instances.clear();

        for (std::size_t i = 0; i < count; ++i)
        {
            const Particle& particle = particles[i];
            const float t = particle.age / particle.lifetime;  // 0 = just spawned, 1 = about to die

            m_instances.push_back({ particle.position,
                                   glm::mix(settings.startSize, settings.endSize, t),
                                   glm::mix(settings.startColor, settings.endColor, t) });
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVbo);
        glBufferData(GL_ARRAY_BUFFER, m_instances.size() * sizeof(Instance), m_instances.data(), GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (settings.blend == ParticleBlend::Additive)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        }

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, static_cast<GLsizei>(count));

        // Back to the normal blend mode the Renderer uses for everything else.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}