#pragma once

#include <glm/vec3.hpp>

namespace MiraEngine
{
    struct AmbientLight
    {
        glm::vec3 color{ 0.2f, 0.2f, 0.2f };
    };

    struct DirectionalLight
    {
        glm::vec3 direction{ -0.5f, -1.0f, -0.3f };
        glm::vec3 color{ 0.8f, 0.8f, 0.8f };
    };
}