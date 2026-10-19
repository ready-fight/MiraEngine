#pragma once

#include "Scene/GameObject.h"

#include <glm/vec3.hpp>

#include <cstddef>

namespace MiraEngine
{
    // A one-shot effect: emits all its particles at once, then removes itself once they've all died.
    class ParticleBurst : public GameObject
    {
    public:
        ParticleBurst(const ParticleSettings& settings, const glm::vec3& position, std::size_t count);
        void Update(float deltaTime) override;
    };
}