#pragma once

#include <glm/vec3.hpp>

namespace MiraEngine
{
    struct AmbientLight
    {
        glm::vec3 color{0.7f, 0.7f, 0.7f};
    };

    struct DirectionalLight
    {
        glm::vec3 direction{-0.5f, -1.0f, -0.3f};
        glm::vec3 color{0.65f, 0.65f, 0.65f};
    };

    struct Fog
    {
        glm::vec3 color{ 0.55f, 0.7f, 0.9f }; // also the sky color, so distant things fade into it
        float start = 8.0f;                  // no fog closer than this
        float end = 30.0f;                   // fully fogged at this distance
    };
}