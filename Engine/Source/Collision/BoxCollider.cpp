#include "Collision/BoxCollider.h"
#include "Collision/SphereCollider.h"

#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace MiraEngine
{
    BoxCollider::BoxCollider(
        const glm::vec3& halfExtents
    )
        : m_halfExtents(halfExtents)
    {
    }

    void BoxCollider::SetHalfExtents(
        const glm::vec3& halfExtents
    )
    {
        m_halfExtents = halfExtents;
    }

    const glm::vec3&
        BoxCollider::GetHalfExtents() const
    {
        return m_halfExtents;
    }

    bool BoxCollider::Intersects(
        const glm::vec3& position,
        const BoxCollider& other,
        const glm::vec3& otherPosition
    ) const
    {
        const glm::vec3 distance =
            glm::abs(
                position - otherPosition
            );

        const glm::vec3 combined =
            m_halfExtents +
            other.m_halfExtents;

        return
            distance.x < combined.x &&
            distance.y < combined.y &&
            distance.z < combined.z;
    }

    bool BoxCollider::Intersects(
        const glm::vec3& position,
        const SphereCollider& sphere,
        const glm::vec3& spherePosition
    ) const
    {
        const glm::vec3 minimum =
            position - m_halfExtents;

        const glm::vec3 maximum =
            position + m_halfExtents;

        const glm::vec3 closestPoint =
            glm::clamp(
                spherePosition,
                minimum,
                maximum
            );

        const glm::vec3 difference =
            spherePosition -
            closestPoint;

        const float distanceSquared =
            glm::dot(
                difference,
                difference
            );

        const float radius =
            sphere.GetRadius();

        return distanceSquared <
            radius * radius;
    }
}