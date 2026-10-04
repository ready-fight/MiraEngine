#pragma once

#include <glm/vec3.hpp>

namespace MiraEngine
{
    struct Material
    {
        glm::vec3 color{ 1.0f, 1.0f, 1.0f };
        float shininess = 32.0f;
    };
}