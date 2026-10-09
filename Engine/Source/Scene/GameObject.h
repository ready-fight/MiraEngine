#pragma once

#include "Graphics/Material.h"
#include "Scene/Transform.h"
#include "Scene/Scene.h"

#include <memory>
#include <string>
#include <vector>

namespace MiraEngine
{
    class Collider;
    class Model;
    class Animator;
    class Texture;

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

        void SetCollider(std::unique_ptr<Collider> collider);

        Collider* GetCollider();
        const Collider* GetCollider() const;

        void SetModel(std::shared_ptr<Model> model);
        void SetDiffuseOverride(
            const std::string& path
        );
        const std::shared_ptr<Texture>&
            GameObject::GetDiffuseOverride() const;

        Model* GetModel();
        const Model* GetModel() const;

        Animator* GetAnimator();
        const Animator* GetAnimator() const;

        Material& GetMaterial();
        const Material& GetMaterial() const;

    private:
        std::string m_name;
        const Scene* m_scene;
        Transform m_transform;

        std::unique_ptr<Collider> m_collider;
        std::unique_ptr<Animator> m_animator;
        std::shared_ptr<Model> m_model;
        std::shared_ptr<Texture> m_diffuseOverride;
        Material m_material;

        float m_fadeTimer = 0.0f;
        const float m_fadeTime = 0.5f;
        const float m_fadeSpeed = 3.0f;
        bool m_pendingDestroy = false;
    };
}