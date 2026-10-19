#include "Effects/Effects.h"

#include "Particles/ParticleBurst.h"
#include "Particles/ParticleEmitter.h"
#include "Scene/Scene.h"

#include <memory>

namespace MiraGame
{
    void SpawnHitSparks(MiraEngine::Scene& scene, const glm::vec3& position)
    {
        // Small, fast, short-lived sparks that spray out and drop.
        MiraEngine::ParticleSettings sparks;
        sparks.maxParticles = 30;
        sparks.minLifetime = 0.2f;
        sparks.maxLifetime = 0.4f;
        sparks.minVelocity = { -3.0f, 0.5f, -3.0f };
        sparks.maxVelocity = { 3.0f, 3.0f, 3.0f };
        sparks.gravity = { 0.0f, -9.8f, 0.0f };
        sparks.drag = 2.0f;
        sparks.startSize = 0.08f;
        sparks.endSize = 0.0f;
        sparks.startColor = { 1.0f, 0.85f, 0.4f, 1.0f };
        sparks.endColor = { 1.0f, 0.3f, 0.0f, 0.0f };
        sparks.blend = MiraEngine::ParticleBlend::Additive;

        scene.AddObject(std::make_unique<MiraEngine::ParticleBurst>(sparks, position, 20));
    }
}