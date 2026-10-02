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
        layout(location = 3) in ivec4 boneIDs;
        layout(location = 4) in vec4 weights;

        const int MAX_BONES = 100;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform mat4 boneMatrices[MAX_BONES];

        out vec3 worldNormal;

        void main()
        {
            vec4 localPosition = vec4(position, 1.0);
            vec3 localNormal = normal;

            float totalWeight =
                weights.x +
                weights.y +
                weights.z +
                weights.w;

            if (totalWeight > 0.0)
            {
                mat4 skinMatrix =
                    boneMatrices[boneIDs.x] * weights.x +
                    boneMatrices[boneIDs.y] * weights.y +
                    boneMatrices[boneIDs.z] * weights.z +
                    boneMatrices[boneIDs.w] * weights.w;

                localPosition = skinMatrix * localPosition;
                localNormal = mat3(skinMatrix) * localNormal;
            }

            worldNormal =
                mat3(transpose(inverse(model))) * localNormal;

            gl_Position =
                projection *
                view *
                model *
                localPosition;
        }
    )";

	const char* FragmentShaderSource = R"(
        #version 460 core

	    uniform vec3 baseColor;

        in vec3 worldNormal;
        out vec4 fragmentColor;

        void main()
        {
            vec3 normalizedNormal = normalize(worldNormal);

            vec3 directionToLight = normalize(
                vec3(0.5, 1.0, 0.3)
            );

            float diffuseAmount = max(
                dot(normalizedNormal, directionToLight),
                0.0
            );

            float lighting = 0.2 + diffuseAmount * 0.8;;

            fragmentColor = vec4(
                baseColor * lighting,
                1.0
            );
        }
    )";
}

namespace Mira {
	Renderer::Renderer() : m_shader(VertexShaderSource, FragmentShaderSource)
	{
		glEnable(GL_DEPTH_TEST);
	}

	Renderer::~Renderer()
	{
		
	}

	void Renderer::Render(float aspectRatio, Camera& camera, Scene& scene)
	{
		glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader.Bind();

		const glm::mat4 view = camera.GetViewMatrix();
		m_shader.SetMatrix4("view", view);

		const glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);
		m_shader.SetMatrix4("projection", projection);

		for (const auto& object : scene.GetObjects())
		{
			glm::mat4 theMatrix = object->GetTransform().GetMatrix();

			m_shader.SetMatrix4("model", theMatrix);

			const int location = glGetUniformLocation(m_shader.GetProgram(), "baseColor");;
			glUniform3f(location, 255, 255, 255);
		}

		m_shader.Unbind();
		
		
	}

    void Renderer::RenderModel(
        float aspectRatio,
        Camera& camera,
        const Transform& transform,
        const Model& model,
        const Animator& animator
    )
    {
        m_shader.Bind();

        m_shader.SetMatrix4(
            "view",
            camera.GetViewMatrix()
        );

        m_shader.SetMatrix4(
            "projection",
            camera.GetProjectionMatrix(aspectRatio)
        );

        m_shader.SetMatrix4(
            "model",
            transform.GetMatrix()
        );

        m_shader.SetMatrix4Array(
            "boneMatrices[0]",
            animator.GetFinalBoneMatrices()
        );

        const int location =
            glGetUniformLocation(
                m_shader.GetProgram(),
                "baseColor"
            );

        glUniform3f(location, 1.0f, 1.0f, 1.0f);

        model.Draw();

        m_shader.Unbind();
    }
}

