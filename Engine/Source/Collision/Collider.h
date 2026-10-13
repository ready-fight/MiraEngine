#pragma once

#include <glm/vec3.hpp>

namespace MiraEngine
{
    struct Collider
    {
        enum class Shape
        {
            Sphere,
            Box,
        };

        Shape shape = Shape::Sphere;
        float radius = 0.5f;           // Sphere only
        glm::vec3 halfExtents{ 0.5f };   // Box only

        static Collider Sphere(float radius);
        static Collider Box(const glm::vec3& halfExtents);
    };

    bool Intersects(const Collider& a, const glm::vec3& positionA, const Collider& b, const glm::vec3& positionB);
}