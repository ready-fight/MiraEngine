#include "Particles/ParticleEmitter.h"

#include <cmath>

namespace MiraEngine
{
    ParticleEmitter::ParticleEmitter(const ParticleSettings& settings)
        : m_settings(settings)
        , m_particles(settings.maxParticles)
    {
    }

    void ParticleEmitter::Update(float deltaTime)
    {
        // Leftover fractions carry over to the next frame, so 20 per second
        // stays 20 per second at any frame rate.
        m_spawnAccumulator += m_settings.spawnRate * deltaTime;

        while (m_spawnAccumulator >= 1.0f)
        {
            Spawn();
            m_spawnAccumulator -= 1.0f;
        }

        for (std::size_t i = 0; i < m_aliveCount;)
        {
            Particle& particle = m_particles[i];
            particle.age += deltaTime;

            if (particle.age >= particle.lifetime)
            {
                // Move the last alive particle into this slot so alive particles stay packed at the front.
                // Don't advance i: the particle we just moved here still needs updating.
                particle = m_particles[m_aliveCount - 1];
                --m_aliveCount;
                continue;
            }

            particle.velocity += m_settings.gravity * deltaTime;
            particle.velocity *= std::exp(-m_settings.drag * deltaTime);
            particle.position += particle.velocity * deltaTime;
            ++i;
        }
    }

    void ParticleEmitter::Emit(std::size_t count)
    {
        for (std::size_t i = 0; i < count; ++i)
        {
            Spawn();
        }
    }

    void ParticleEmitter::Spawn()
    {
        // Pool is full: skip rather than allocate.
        if (m_aliveCount >= m_particles.size())
        {
            return;
        }

        Particle& particle = m_particles[m_aliveCount];
        ++m_aliveCount;

        particle.position = m_position;
        particle.velocity = glm::vec3(RandomRange(m_settings.minVelocity.x, m_settings.maxVelocity.x),
            RandomRange(m_settings.minVelocity.y, m_settings.maxVelocity.y),
            RandomRange(m_settings.minVelocity.z, m_settings.maxVelocity.z));
        particle.age = 0.0f;
        particle.lifetime = RandomRange(m_settings.minLifetime, m_settings.maxLifetime);
    }

    float ParticleEmitter::RandomRange(float min, float max)
    {
        // Scaling a 0..1 roll also works when min == max; uniform_real_distribution(min, max) can assert then.
        std::uniform_real_distribution<float> zeroToOne(0.0f, 1.0f);
        return min + (max - min) * zeroToOne(m_random);
    }
}