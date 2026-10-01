#include "Scene/GameObject.h"

#include "Collision/Collider.h"

namespace Mira
{
    GameObject::GameObject(const std::string& name)
        : m_name(name)
    {
    }

    Transform& GameObject::GetTransform()
    {
        return m_transform;
    }

    const Transform& GameObject::GetTransform() const
    {
        return m_transform;
    }

    const std::string& GameObject::GetName() const
    {
        return m_name;
    }

    void GameObject::SetCollider(std::unique_ptr<Collider> collider)
    {
        m_collider = std::move(collider);
    }

    Collider* GameObject::GetCollider()
    {
        return m_collider.get();
    }

    const Collider* GameObject::GetCollider() const
    {
        return m_collider.get();
    }
}