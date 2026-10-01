#include "Collision/SphereCollider.h"

#include <glm/geometric.hpp>

namespace Mira
{
    SphereCollider::SphereCollider(float radius)
        : m_radius(radius)
    {
    }

    void SphereCollider::SetRadius(float radius)
    {
        m_radius = radius;
    }

    float SphereCollider::GetRadius() const
    {
        return m_radius;
    }

    bool SphereCollider::Intersects(
        const glm::vec3& position,
        const SphereCollider& other,
        const glm::vec3& otherPosition
    ) const
    {
        const float distance =
            glm::distance(position, otherPosition);

        return distance < m_radius + other.m_radius;
    }
}