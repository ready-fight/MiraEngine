#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace MiraEngine
{
    struct Material
    {
        glm::vec4 color{ 1.0f };
        float shininess = 32.0f;
    };
}