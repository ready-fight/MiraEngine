#pragma once

#include "Collision/Collider.h"
#include <glm/vec3.hpp>

namespace MiraEngine
{
    class SphereCollider : public Collider
    {
    public:
        SphereCollider(float radius = 0.5f);

        void SetRadius(float radius);
        float GetRadius() const;

        bool Intersects(
            const glm::vec3& position,
            const SphereCollider& other,
            const glm::vec3& otherPosition
        ) const;

    private:
        float m_radius;
    };
}