#pragma once

#include "Graphics/Material.h"
#include "Scene/Transform.h"

#include <memory>
#include <string>

namespace MiraEngine
{
    class Collider;
    class Model;
    class Animator;

    class GameObject
    {
    public:
        GameObject(const std::string& name = "GameObject");
        virtual ~GameObject();
        virtual void Update(float deltaTime);
        virtual void DrawUI();

        Transform& GetTransform();
        const Transform& GetTransform() const;

        const std::string& GetName() const;

        void SetCollider(std::unique_ptr<Collider> collider);

        Collider* GetCollider();
        const Collider* GetCollider() const;

        void SetModel(std::shared_ptr<Model> model);

        Model* GetModel();
        const Model* GetModel() const;

        Animator* GetAnimator();
        const Animator* GetAnimator() const;

        Material& GetMaterial();
        const Material& GetMaterial() const;

    private:
        std::string m_name;
        Transform m_transform;

        std::unique_ptr<Collider> m_collider;
        std::shared_ptr<Model> m_model;
        std::unique_ptr<Animator> m_animator;
        Material m_material;
    };
}