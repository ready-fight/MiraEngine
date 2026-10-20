#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Animation/Animator.h"
#include "Collision/Collider.h"

#include <algorithm>

namespace MiraEngine
{
    void Scene::AddObject(std::unique_ptr<GameObject> object)
    {
        const std::string name = object->GetName();
        int& count = m_nameCounts[name];

        if (count > 0)
        {
            object->SetName(name + " " + std::to_string(count));
        }

        ++count;

        // Adding to m_objects in the middle of Update would invalidate the loop over it,
        // so objects created during Update join the scene once the loop is done.
        if (m_isUpdating)
        {
            m_pendingObjects.push_back(std::move(object));
        }
        else
        {
            m_objects.push_back(std::move(object));
        }
    }

    void Scene::DrawUI()
    {
        for (const auto& object : m_objects)
        {
            object->DrawUI();
        }
    }

    std::vector<std::unique_ptr<GameObject>>& Scene::GetObjects()
    {
        return m_objects;
    }

    const std::vector<std::unique_ptr<GameObject>>& Scene::GetObjects() const
    {
        return m_objects;
    }

    bool Scene::Overlaps(const GameObject& a, const GameObject& b) const
    {
        const Collider* colliderA = a.GetCollider();
        const Collider* colliderB = b.GetCollider();

        if (!colliderA || !colliderB)
        {
            return false;
        }

        const Transform& transformA = a.GetTransform();
        const Transform& transformB = b.GetTransform();

        return Intersects(*colliderA, transformA.GetPosition(), transformA.GetRotation().y,
            *colliderB, transformB.GetPosition(), transformB.GetRotation().y);
    }

    bool Scene::IsColliding(const GameObject& a, const GameObject& b) const
    {
        const Collider* colliderA = a.GetCollider();
        const Collider* colliderB = b.GetCollider();

        if (!colliderA || !colliderB || colliderA->isTrigger || colliderB->isTrigger)
        {
            return false;
        }

        return Overlaps(a, b);
    }

    bool Scene::IsColliding(const GameObject& object) const
    {
        for (const auto& other : m_objects)
        {
            if (other.get() == &object)
            {
                continue;
            }

            if (IsColliding(object, *other))
            {
                return true;
            }
        }

        return false;
    }

    bool Scene::TryMove(GameObject& object, const glm::vec3& movement)
    {
        Transform& transform = object.GetTransform();
        const glm::vec3 oldPosition = transform.GetPosition();
        transform.SetPosition(oldPosition + movement);

        if (IsColliding(object))
        {
            transform.SetPosition(oldPosition);
            return false;
        }

        return true;
    }

    void Scene::Update(float deltaTime)
    {
        m_isUpdating = true;

        for (const auto& object : m_objects)
        {
            object->Update(deltaTime);
            Animator* animator = object->GetAnimator();

            if (animator)
            {
                animator->Update(deltaTime);
            }
        }

        m_isUpdating = false;

        for (auto& object : m_pendingObjects)
        {
            m_objects.push_back(std::move(object));
        }

        m_pendingObjects.clear();

        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](const std::unique_ptr<GameObject>& object)
                                       { return object->IsPendingDestroy(); }),
                        m_objects.end());
    }

    void Scene::Clear()
    {
        m_objects.clear();
        m_pendingObjects.clear();
        m_nameCounts.clear();
    }
}