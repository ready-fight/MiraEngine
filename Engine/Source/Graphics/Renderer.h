#pragma once

#include "Shader.h"
#include "Graphics/ParticleRenderer.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/Light.h"

namespace MiraEngine
{

    class Camera;
    class Scene;
    class Model;
    class Transform;
    class Animator;

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        void Render(float aspectRatio, Camera& camera, Scene& scene);
        void SetAmbientLight(const AmbientLight& light);
        void SetDirectionalLight(const DirectionalLight& light);
        void SetFog(const Fog& fog);
        void SetDebugColliders(bool enabled);
        bool GetDebugColliders() const { return m_debugColliders; }

    private:
        Shader m_shader;
        AmbientLight m_ambientLight;
        DirectionalLight m_directionalLight;
        ParticleRenderer m_particleRenderer;
        DebugRenderer m_debugRenderer;
		Fog m_fog;
        bool m_debugColliders = false;
    };
}
