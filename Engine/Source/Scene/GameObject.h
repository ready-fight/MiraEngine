#pragma once

#include "Graphics/Material.h"
#include "Scene/Transform.h"
#include "Scene/Scene.h"
#include "Collision/Collider.h"

#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace MiraEngine
{
    class Model;
    class Animator;

    class GameObject
    {
    public:
        GameObject(const std::string& name = "GameObject");
        virtual ~GameObject();
        virtual void Update(float deltaTime);
        virtual void DrawUI();
        void FadeIn(float deltaTime);
        void FadeOut(float deltaTime);
        bool IsPendingDestroy() const;
        Transform& GetTransform();
        const Transform& GetTransform() const;
        const std::string& GetName() const;
        void SetCollider(const Collider& collider);
        void RemoveCollider();
        Collider* GetCollider();
        const Collider* GetCollider() const;
        void SetName(const std::string& name);
        void SetModel(const std::string& path);
        void SetTexture(TextureSlot slot, const std::string& path);
        Model* GetModel();
        const Model* GetModel() const;
        Animator* GetAnimator();
        const Animator* GetAnimator() const;
        Material& GetMaterial();
        const Material& GetMaterial() const;
        void PlayAnimation(const std::string& name, bool looping = true);
        void Destroy();

    private:
        std::string m_name;
        Transform m_transform;
        std::optional<Collider> m_collider;
        std::unique_ptr<Animator> m_animator;
        std::shared_ptr<Model> m_model;
        Material m_material;
        const float m_fadeSpeed = 3.0f;
        bool m_pendingDestroy = false;
    };
}