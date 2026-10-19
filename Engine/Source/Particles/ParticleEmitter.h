#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstddef>
#include <random>
#include <vector>

namespace MiraEngine
{
    struct Particle
    {
        glm::vec3 position{ 0.0f };
        glm::vec3 velocity{ 0.0f };
        float age = 0.0f;
        float lifetime = 1.0f;
    };

    // Alpha: normal see-through (smoke, dust). Additive: brightens what's behind it (fire, sparks, magic).
    enum class ParticleBlend
    {
        Alpha,
        Additive
    };

    struct ParticleSettings
    {
        std::size_t maxParticles = 500;
        float spawnRate = 20.0f;  // particles per second; 0 = only spawn through Emit()
        float minLifetime = 1.0f;
        float maxLifetime = 2.0f;
        glm::vec3 minVelocity{ -0.5f, 2.0f, -0.5f };
        glm::vec3 maxVelocity{ 0.5f, 3.0f, 0.5f };
        glm::vec3 gravity{ 0.0f, -9.8f, 0.0f };
        float startSize = 0.2f;
        float endSize = 0.0f;
        glm::vec4 startColor{ 1.0f };
        glm::vec4 endColor{ 1.0f, 1.0f, 1.0f, 0.0f };  // fades out by default
        float drag = 0.0f;                           // how quickly particles slow down; 0 = never
        ParticleBlend blend = ParticleBlend::Alpha;
    };

    class ParticleEmitter
    {
    public:
        explicit ParticleEmitter(const ParticleSettings& settings);

        void Update(float deltaTime);
        void Emit(std::size_t count);
        void SetPosition(const glm::vec3& position) { m_position = position; }

        const ParticleSettings& GetSettings() const { return m_settings; }
        // Only the first GetAliveCount() particles are alive; the rest of the pool is unused.
        const std::vector<Particle>& GetParticles() const { return m_particles; }
        std::size_t GetAliveCount() const { return m_aliveCount; }

    private:
        void Spawn();
        float RandomRange(float min, float max);

        ParticleSettings m_settings;
        glm::vec3 m_position{ 0.0f };
        std::vector<Particle> m_particles;
        std::size_t m_aliveCount = 0;
        float m_spawnAccumulator = 0.0f;
        std::mt19937 m_random{ std::random_device{}() };
    };
}