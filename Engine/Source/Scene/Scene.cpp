#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Animation/Animator.h"
#include "Particles/ParticleEmitter.h"
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

    bool Scene::IsColliding(const GameObject& a, const GameObject& b) const
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
        for (const auto& object : m_objects)
        {
            object->Update(deltaTime);
            Animator* animator = object->GetAnimator();

            if (animator)
            {
                animator->Update(deltaTime);
            }

            ParticleEmitter* emitter = object->GetParticleEmitter();

            if (emitter)
            {
                // After object->Update, so new particles spawn where the object is this frame.
                emitter->SetPosition(object->GetTransform().GetPosition());
                emitter->Update(deltaTime);
            }
        }

        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [](const std::unique_ptr<GameObject>& object)
                                       { return object->IsPendingDestroy(); }),
                        m_objects.end());
    }
}