#include "Particles/ParticleBurst.h"
#include "Particles/ParticleEmitter.h"

namespace MiraEngine
{
    ParticleBurst::ParticleBurst(const ParticleSettings& settings, const glm::vec3& position, std::size_t count)
        : GameObject("ParticleBurst")
    {
        // A burst only emits once; continuous spawning would keep it alive forever.
        ParticleSettings burstSettings = settings;
        burstSettings.spawnRate = 0.0f;

        GetTransform().SetPosition(position);
        SetParticleEmitter(burstSettings);

        // The scene only syncs the emitter's position during Update, but we emit right now.
        ParticleEmitter* emitter = GetParticleEmitter();
        emitter->SetPosition(position);
        emitter->Emit(count);
    }

    void ParticleBurst::Update(float /*deltaTime*/)
    {
        if (GetParticleEmitter()->GetAliveCount() == 0)
        {
            Destroy();
        }
    }
}