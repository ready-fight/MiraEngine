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

    struct ParticleSettings
    {
        std::size_t maxParticles = 500;
        float spawnRate = 20.0f;
        float minLifetime = 1.0f;
        float maxLifetime = 2.0f;
        glm::vec3 minVelocity{ -0.5f, 2.0f, -0.5f };
        glm::vec3 maxVelocity{ 0.5f, 3.0f, 0.5f };
        glm::vec3 gravity{ 0.0f, -9.8f, 0.0f };
        float size = 0.1f;
        glm::vec4 color{ 1.0f };
    };
}