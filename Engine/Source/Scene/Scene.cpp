#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Animation/Animator.h"

#include "Collision/Collider.h"
#include "Collision/BoxCollider.h"
#include "Collision/SphereCollider.h"

#include <algorithm>

namespace MiraEngine
{
    void Scene::AddObject(std::unique_ptr<GameObject> object)
    {
        m_objects.push_back(std::move(object));
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

    bool Scene::IsColliding(
        const GameObject& a,
        const GameObject& b
    ) const
    {
        const Collider* colliderA =
            a.GetCollider();

        const Collider* colliderB =
            b.GetCollider();

        if (!colliderA || !colliderB)
        {
            return false;
        }

        const glm::vec3 positionA =
            a.GetTransform().GetPosition();

        const glm::vec3 positionB =
            b.GetTransform().GetPosition();

        const auto* sphereA =
            dynamic_cast<
            const SphereCollider*
            >(colliderA);

        const auto* sphereB =
            dynamic_cast<
            const SphereCollider*
            >(colliderB);

        const auto* boxA =
            dynamic_cast<
            const BoxCollider*
            >(colliderA);

        const auto* boxB =
            dynamic_cast<
            const BoxCollider*
            >(colliderB);

        // Sphere vs Sphere
        if (sphereA && sphereB)
        {
            return sphereA->Intersects(
                positionA,
                *sphereB,
                positionB
            );
        }

        // Box vs Box
        if (boxA && boxB)
        {
            return boxA->Intersects(
                positionA,
                *boxB,
                positionB
            );
        }

        // Sphere A vs Box B
        if (sphereA && boxB)
        {
            return boxB->Intersects(
                positionB,
                *sphereA,
                positionA
            );
        }

        // Box A vs Sphere B
        if (boxA && sphereB)
        {
            return boxA->Intersects(
                positionA,
                *sphereB,
                positionB
            );
        }

        return false;
    }

    bool Scene::IsColliding(
        const GameObject& object
    ) const
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

    bool Scene::TryMove(
        GameObject& object,
        const glm::vec3& movement
    )
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
        for (const auto& object : m_objects)
        {
            object->Update(deltaTime);

            Animator* animator = object->GetAnimator();

            if (animator)
            {
                animator->Update(deltaTime);
            }
        }

        m_objects.erase(
            std::remove_if(
                m_objects.begin(),
                m_objects.end(),
                [](const std::unique_ptr<GameObject>& object)
                {
                    return object->IsPendingDestroy();
                }
            ),
            m_objects.end()
        );
    }
}