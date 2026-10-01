#pragma once

#include "Scene/Transform.h"

#include <memory>
#include <string>

namespace Mira
{
    class Collider;

    class GameObject
    {
    public:
        GameObject(const std::string& name = "GameObject");
        virtual ~GameObject() = default;

        Transform& GetTransform();
        const Transform& GetTransform() const;

        const std::string& GetName() const;

        void SetCollider(std::unique_ptr<Collider> collider);

        Collider* GetCollider();
        const Collider* GetCollider() const;

    private:
        std::string m_name;
        Transform m_transform;

        std::unique_ptr<Collider> m_collider;
    };
}