#pragma once

#include "Collision/Collider.h"

#include <glm/vec3.hpp>

namespace MiraEngine
{
    class SphereCollider;

    class BoxCollider : public Collider
    {
    public:
        BoxCollider(
            const glm::vec3& halfExtents =
            glm::vec3(0.5f)
        );

        void SetHalfExtents(
            const glm::vec3& halfExtents
        );

        const glm::vec3&
            GetHalfExtents() const;

        bool Intersects(
            const glm::vec3& position,
            const BoxCollider& other,
            const glm::vec3& otherPosition
        ) const;

        bool Intersects(
            const glm::vec3& position,
            const SphereCollider& sphere,
            const glm::vec3& spherePosition
        ) const;

    private:
        glm::vec3 m_halfExtents;
    };
}