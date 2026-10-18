#include "Collision/Collider.h"

#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

#include <cmath>
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

        // Moves a world point into a box's own unrotated space (undoes the box's yaw around its center).
        glm::vec3 ToBoxSpace(const glm::vec3& point, const glm::vec3& boxCenter, float yawDegrees)
        {
            const float yaw = glm::radians(yawDegrees);
            const float c = std::cos(yaw);
            const float s = std::sin(yaw);
            const glm::vec3 d = point - boxCenter;
            return glm::vec3(c * d.x - s * d.z, d.y, s * d.x + c * d.z);
        }

        // Half extents of the axis-aligned box that contains a yaw-rotated box.
        glm::vec3 RotatedExtents(const glm::vec3& halfExtents, float yawDegrees)
        {
            const float yaw = glm::radians(yawDegrees);
            const float c = std::abs(std::cos(yaw));
            const float s = std::abs(std::sin(yaw));
            return glm::vec3(c * halfExtents.x + s * halfExtents.z, halfExtents.y, s * halfExtents.x + c * halfExtents.z);
        }

        bool BoxVsSphere(const glm::vec3& box, const glm::vec3& halfExtents, float yaw, const glm::vec3& sphere, float radius)
        {
            // In the box's own space it's axis-aligned again, so the usual test works.
            const glm::vec3 local = ToBoxSpace(sphere, box, yaw);
            const glm::vec3 closest = glm::clamp(local, -halfExtents, halfExtents);
            const glm::vec3 d = local - closest;
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

    bool Intersects(const Collider& a, const glm::vec3& positionA, float yawA,
        const Collider& b, const glm::vec3& positionB, float yawB)
    {
        using Shape = Collider::Shape;

        if (a.shape == Shape::Sphere && b.shape == Shape::Sphere)
        {
            return SphereVsSphere(positionA, a.radius, positionB, b.radius);
        }
        if (a.shape == Shape::Box && b.shape == Shape::Box)
        {
            // Approximate: compares the axis-aligned boxes that contain each rotated box.
            return BoxVsBox(positionA, RotatedExtents(a.halfExtents, yawA), positionB, RotatedExtents(b.halfExtents, yawB));
        }
        if (a.shape == Shape::Box)
        {
            return BoxVsSphere(positionA, a.halfExtents, yawA, positionB, b.radius);
        }
        return BoxVsSphere(positionB, b.halfExtents, yawB, positionA, a.radius);
    }
}