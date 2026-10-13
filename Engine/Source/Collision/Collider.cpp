#include "Collision/Collider.h"

#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace MiraEngine
{
    namespace
    {
        bool SphereVsSphere(const glm::vec3& a, float radiusA, const glm::vec3& b, float radiusB)
        {
            const glm::vec3 d = a - b;
            const float r = radiusA + radiusB;
            return glm::dot(d, d) < r * r;
        }

        bool BoxVsBox(const glm::vec3& a, const glm::vec3& halfA, const glm::vec3& b, const glm::vec3& halfB)
        {
            const glm::vec3 d = glm::abs(a - b);
            const glm::vec3 r = halfA + halfB;
            return d.x < r.x && d.y < r.y && d.z < r.z;
        }

        bool BoxVsSphere(const glm::vec3& box, const glm::vec3& halfExtents, const glm::vec3& sphere, float radius)
        {
            // Closest point on the box to the sphere's center.
            const glm::vec3 closest = glm::clamp(sphere, box - halfExtents, box + halfExtents);
            const glm::vec3 d = sphere - closest;
            return glm::dot(d, d) < radius * radius;
        }
    }

    Collider Collider::Sphere(float radius)
    {
        Collider collider;
        collider.shape = Shape::Sphere;
        collider.radius = radius;
        return collider;
    }

    Collider Collider::Box(const glm::vec3& halfExtents)
    {
        Collider collider;
        collider.shape = Shape::Box;
        collider.halfExtents = halfExtents;
        return collider;
    }

    bool Intersects(const Collider& a, const glm::vec3& positionA, const Collider& b, const glm::vec3& positionB)
    {
        using Shape = Collider::Shape;

        if (a.shape == Shape::Sphere && b.shape == Shape::Sphere)
        {
            return SphereVsSphere(positionA, a.radius, positionB, b.radius);
        }
        if (a.shape == Shape::Box && b.shape == Shape::Box)
        {
            return BoxVsBox(positionA, a.halfExtents, positionB, b.halfExtents);
        }
        if (a.shape == Shape::Box)
        {
            return BoxVsSphere(positionA, a.halfExtents, positionB, b.radius);
        }
        return BoxVsSphere(positionB, b.halfExtents, positionA, a.radius);
    }
}