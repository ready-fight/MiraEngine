#include "Application.h"
#include "GameObject.h"

#include <glm/vec3.hpp>

#include <chrono>


namespace Mira 
{
	Application::Application() : m_window(1280, 720, "Mira Engine"), m_camera(
			glm::vec3(1.5f, 1.2f, 3.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f)
		)
	{

		std::shared_ptr<Model> model = std::make_shared<Model>(
			"Assets/Models/FinalBaseMesh.obj"
		);

		GameObject object(
			model,
			Transform(
				glm::vec3(0.0f),
				glm::vec3(0.0f, -20.0f, 0.0f),
				glm::vec3(0.05f)
			)
		);

		m_scene.AddObject(object);

		m_window.CaptureCursor();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{

		auto previousTime = std::chrono::steady_clock::now();
		float movementSpeed = 2.0f;


		while (!m_window.ShouldClose())
		{

			const auto currentTime = std::chrono::steady_clock::now();

			const float deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();

			previousTime = currentTime;

			m_window.ProcessEvents();

			const MouseMovement mouseMovement = m_window.GetMouseMovement();

			const float mouseSensitivity = 0.1f;

			m_camera.Rotate(
				static_cast<float>(mouseMovement.x) * mouseSensitivity,
				static_cast<float>(mouseMovement.y) * mouseSensitivity
			);

			glm::vec3 movement(0.0f);

			if (m_window.IsKeyPressed(Key::W))
			{
				movement.z +=  1.0f;
			}

			if (m_window.IsKeyPressed(Key::S))
			{
				movement.z -= 1.0f;
			}

			if (m_window.IsKeyPressed(Key::A))
			{
				movement.x -= 1.0f;
			}

			if (m_window.IsKeyPressed(Key::D))
			{
				movement.x += 1.0f;
			}

			if (m_window.IsKeyPressed(Key::Q))
			{
				movement.y -= 1.0f;
			}

			if (m_window.IsKeyPressed(Key::E))
			{
				movement.y += 1.0f;
			}

			if (glm::length(movement) > 0.0f) {
				movement = glm::normalize(movement);


				const float distance = movementSpeed * deltaTime;

				m_camera.MoveRelative(movement.z * distance, movement.x * distance, movement.y * distance);
			}

			m_renderer.Render(m_window.GetAspectRatio(), m_camera, m_scene);

			m_window.SwapBuffers();
		}
	}
}
