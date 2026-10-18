#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Graphics/Model.h"
#include "Scene/Transform.h"
#include "Animation/Animator.h"

#include <GL/glew.h>

#include <cstdint>
#include <vector>

namespace
{
    const char* VertexShaderSource = R"(
        #version 460 core

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 texCoord;
        layout(location = 3) in ivec4 boneIDs;
        layout(location = 4) in vec4 weights;
        layout(location = 5) in vec4 tangent;

        const int MAX_BONES = 100;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform mat4 boneMatrices[MAX_BONES];

        out vec3 worldNormal;
        out vec2 textureCoordinate;
        out vec4 worldTangent;
        out vec3 worldPosition;

        void main()
        {
            vec4 localPosition = vec4(position, 1.0);
            vec3 localNormal = normal;
            vec3 localTangent = tangent.xyz;

            textureCoordinate = texCoord;

            float totalWeight = weights.x + weights.y + weights.z + weights.w;
            if (totalWeight > 0.0)
            {
                mat4 skinMatrix = boneMatrices[boneIDs.x] * weights.x
                                + boneMatrices[boneIDs.y] * weights.y
                                + boneMatrices[boneIDs.z] * weights.z
                                + boneMatrices[boneIDs.w] * weights.w;

                localPosition = skinMatrix * localPosition;
                localNormal = mat3(skinMatrix) * localNormal;
                localTangent = mat3(skinMatrix) * localTangent;
            }

            worldNormal = mat3(transpose(inverse(model))) * localNormal;
            worldTangent = vec4(mat3(model) * localTangent, tangent.w);worldPosition = vec3(model * localPosition);
            vec4 world = model * localPosition;
            worldPosition = world.xyz;
            gl_Position = projection * view * world;
        }
    )";

    const char* FragmentShaderSource = R"(
        #version 460 core

        uniform vec4 baseColor;
        uniform vec3 ambientLight;
        uniform vec3 lightDirection;
        uniform vec3 lightColor;

        uniform sampler2D colorTexture;
        uniform int hasColorTexture;
        uniform sampler2D normalTexture;
        uniform int hasNormalTexture;
        uniform sampler2D occlusionTexture;
        uniform int hasOcclusionTexture;
        uniform sampler2D roughnessTexture;
        uniform int hasRoughnessTexture;
        uniform vec3 cameraPosition;
        uniform vec3 fogColor;
        uniform float fogStart;
        uniform float fogEnd;

        in vec3 worldNormal;
        in vec4 worldTangent;
        in vec2 textureCoordinate;
        in vec3 worldPosition;

        out vec4 fragmentColor;

        void main()
        {
            vec3 normalizedNormal = normalize(worldNormal);

            if (hasNormalTexture == 1)
            {
                vec3 T = normalize(worldTangent.xyz - normalizedNormal * dot(normalizedNormal, worldTangent.xyz));
                vec3 B = cross(normalizedNormal, T) * worldTangent.w;

                vec3 mapNormal = texture(normalTexture, textureCoordinate).rgb * 2.0 - 1.0;
                normalizedNormal = normalize(mat3(T, B, normalizedNormal) * mapNormal);
            }

            vec3 directionToLight = normalize(-lightDirection);
            float diffuseAmount = max(dot(normalizedNormal, directionToLight), 0.0);

            float occlusion = 1.0;
            if (hasOcclusionTexture == 1)
            {
                occlusion = texture(occlusionTexture, textureCoordinate).r;
            }

            vec3 lighting = ambientLight * occlusion + lightColor * diffuseAmount;

            vec4 surfaceColor = baseColor;
            if (hasColorTexture == 1)
            {
                vec4 texel = texture(colorTexture, textureCoordinate);
                if (texel.a < 0.5)
                {
                    discard;
                }
                surfaceColor.rgb *= texel.rgb;
            }

            vec3 litColor = surfaceColor.rgb * lighting;
            // 0 = no fog (closer than fogStart), 1 = fully fogged (farther than fogEnd)
            float distanceToCamera = length(worldPosition - cameraPosition);
            float fogAmount = clamp((distanceToCamera - fogStart) / (fogEnd - fogStart), 0.0, 1.0);

            fragmentColor = vec4(mix(litColor, fogColor, fogAmount), surfaceColor.a);
        }
    )";
}

namespace MiraEngine
{
    Renderer::Renderer()
        : m_shader(VertexShaderSource, FragmentShaderSource)
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Renderer::~Renderer() {}

    void Renderer::Render(float aspectRatio, Camera& camera, Scene& scene)
    {
        glClearColor(m_fog.color.r, m_fog.color.g, m_fog.color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_shader.Bind();
        const glm::mat4 view = camera.GetViewMatrix();
        m_shader.SetMatrix4("view", view);
        const glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);
        m_shader.SetMatrix4("projection", projection);
        m_shader.SetVector3("ambientLight", m_ambientLight.color);
        m_shader.SetVector3("lightDirection", m_directionalLight.direction);
        m_shader.SetVector3("lightColor", m_directionalLight.color);
        m_shader.SetVector3("cameraPosition", camera.GetPosition());
        m_shader.SetVector3("fogColor", m_fog.color);
        m_shader.SetFloat("fogStart", m_fog.start);
        m_shader.SetFloat("fogEnd", m_fog.end);

        for (const auto& object : scene.GetObjects())
        {
            const Model* model = object->GetModel();

            if (!model)
            {
                continue;
            }

            m_shader.SetMatrix4("model", object->GetTransform().GetMatrix());
            const Animator* animator = object->GetAnimator();

            const std::vector<glm::mat4>* boneTransforms =
                animator ? &animator->GetFinalBoneMatrices() : nullptr;

            const Material& material = object->GetMaterial();
            m_shader.SetVector4("baseColor", material.color);

            model->Draw(m_shader, material.textures, boneTransforms);
        }

        m_shader.Unbind();

        if (m_debugColliders)
        {
            m_debugRenderer.Render(aspectRatio, camera, scene);
        }
    }

    void Renderer::SetAmbientLight(const AmbientLight& light)
    {
        m_ambientLight = light;
    }

    void Renderer::SetDirectionalLight(const DirectionalLight& light)
    {
        m_directionalLight = light;
    }

    void Renderer::SetFog(const Fog& fog)
    {
        m_fog = fog;
    }

    void Renderer::SetDebugColliders(bool enabled)
    {
        m_debugColliders = enabled;
    }
}
