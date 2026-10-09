#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Collision/Collider.h"
#include "Graphics/Model.h"
#include "Graphics/Material.h"
#include "Animation/Animator.h"

namespace MiraEngine
{
    GameObject::GameObject(const std::string& name)
        : m_name(name)
    {
    }

    GameObject::~GameObject() = default;

    void GameObject::Update(float deltaTime)
    {
    }

    void GameObject::DrawUI()
    {
    }

    void GameObject::FadeOut(float deltaTime)
    {
        
        if (m_material.color.a == 0)
        {
            return;
        }


        float alpha = m_material.color.a;
        alpha -= m_fadeSpeed * deltaTime;
        alpha = std::max(alpha, 0.0f);

        m_material.color.a = alpha;

        if (alpha == 0) {
            m_pendingDestroy = true;
        }
    }

    bool GameObject::IsPendingDestroy() const
    {
        return m_pendingDestroy;
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

    void GameObject::SetModel(std::shared_ptr<Model> model)
    {
        m_model = std::move(model);

        if (m_model && !m_model->GetAnimations().empty())
        {
            m_animator = std::make_unique<Animator>(*m_model);
        }
        else
        {
            m_animator.reset();
        }
    }

    void GameObject::SetDiffuseOverride(
        const std::string& path
    )
    {
        m_diffuseOverride =
            std::make_shared<Texture>(
                path
            );
    }

    const std::shared_ptr<Texture>&
        GameObject::GetDiffuseOverride() const
    {
        return m_diffuseOverride;
    }

    Model* GameObject::GetModel()
    {
        return m_model.get();
    }

    const Model* GameObject::GetModel() const
    {
        return m_model.get();
    }

    Animator* GameObject::GetAnimator()
    {
        return m_animator.get();
    }

    const Animator* GameObject::GetAnimator() const
    {
        return m_animator.get();
    }

    Material& GameObject::GetMaterial()
    {
        return m_material;
    }

    const Material& GameObject::GetMaterial() const
    {
        return m_material;
    }
}