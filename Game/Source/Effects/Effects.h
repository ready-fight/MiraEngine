#pragma once

#include <glm/vec3.hpp>

namespace MiraEngine
{
    class Scene;
}

namespace MiraGame
{
    void SpawnHitSparks(MiraEngine::Scene& scene, const glm::vec3& position);
}