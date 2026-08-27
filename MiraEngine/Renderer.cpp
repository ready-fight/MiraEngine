#include "Renderer.h"
#include "Camera.h"

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cstdint>
#include <vector>


namespace
{
	const char* VertexShaderSource = R"(
    #version 460 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	out vec3 worldNormal;

    void main()
    {
		worldNormal = mat3(transpose(inverse(model))) * normal;

        gl_Position = projection * view * model * vec4(position, 1.0);
    }
)";

	const char* FragmentShaderSource = R"(
    #version 460 core

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

        float lighting = 0.2 + diffuseAmount * 0.8;

        vec3 baseColor = vec3(0.9, 0.4, 0.2);

        fragmentColor = vec4(
            baseColor * lighting,
            1.0
        );
    }
)";
}

namespace Wave {
	Renderer::Renderer() : m_shader(VertexShaderSource, FragmentShaderSource), m_model("Assets/Models/FinalBaseMesh.obj")
	{
		glEnable(GL_DEPTH_TEST);
	}

	Renderer::~Renderer()
	{
		
	}

	void Renderer::Render(float aspectRatio, Camera& camera)
	{
		glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader.Bind();

		glm::mat4 model(1.0);
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		m_shader.SetMatrix4("model", model);

		const glm::mat4 view = camera.GetViewMatrix();
		m_shader.SetMatrix4("view", view);

		const glm::mat4 projection = camera.GetProjectMatrix(aspectRatio);
		m_shader.SetMatrix4("projection", projection);

		m_model.Draw();
		m_shader.Unbind();
	}
}

