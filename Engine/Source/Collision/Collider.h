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
        glm::vec3 offset{ 0.0f };   // Centre relative to the object, before its yaw
        bool isTrigger = false;

        static Collider Sphere(float radius);
        static Collider Box(const glm::vec3& halfExtents);
        glm::vec3 GetCenter(const glm::vec3& position, float yawDegrees) const;
    };

    bool Intersects(const Collider& a, const glm::vec3& positionA, float yawA,
                    const Collider& b, const glm::vec3& positionB, float yawB);
}