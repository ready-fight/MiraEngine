#include "Application.h"

#include "GameObject.h"

#include <glm/vec3.hpp>

#include <chrono>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <stdexcept>
#include <string>


namespace Mira 
{
	Application::Application() : m_window(1280, 720, "Mira Engine"), m_camera(
			glm::vec3(-0.00f, 1.23f, -4.41f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		)
	{

		auto characterModel = std::make_shared<Model>(
			"Assets/Models/FinalBaseMesh.obj"
		);

		GameObject player(
			characterModel,
			Transform(
				glm::vec3(0.0f, 0.0f, -2.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.05f)
			),
			glm::vec3(1.0f),
			"Player"
		);

		m_scene.AddObject(player);

		GameObject enemy(
			characterModel,
			Transform(
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, -180.0f, 0.0f),
				glm::vec3(0.05f)
			),
			glm::vec3(1.0f, 0.0f, 0.0f),
			"Enemy"
		);


		m_scene.AddObject(enemy);

		auto platformModel = std::make_shared<Model>(
			"Assets/Models/Cube.obj"
		);

		GameObject platform(
			platformModel,
			Transform(
				glm::vec3(0.0f, -0.1f, 0.0f), // Position
				glm::vec3(0.0f),              // Rotation
				glm::vec3(5.0f, 0.2f, 5.0f)   // Width, height, depth
			),
			glm::vec3(1.0f), // Color
			"Platform"
		);

		m_scene.AddObject(platform);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		if(!ImGui_ImplGlfw_InitForOpenGL(m_window.GetNativeWindow(), true))
		{
			ImGui::DestroyContext();
			throw std::runtime_error("Failed to initialize ImGui GLFW");
		}

		if (!ImGui_ImplOpenGL3_Init("#version 460 core"))
		{
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			throw std::runtime_error("Failed to initialize ImGui OpenGL");
		}

		m_window.ReleaseCursor();
	}

	Application::~Application()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::Run()
	{
		auto previousTime = std::chrono::steady_clock::now();

		const float movementSpeed = 2.0f;
		const float mouseSensitivity = 0.1f;

		int selectedObject = 0;
		bool cameraMode = true;
		bool wasF1Down = false;

		GLFWwindow* window = m_window.GetNativeWindow();

		while (!m_window.ShouldClose())
		{
			const auto currentTime = std::chrono::steady_clock::now();

			const float deltaTime = std::min(
				std::chrono::duration<float>(
					currentTime - previousTime
				).count(),
				0.1f
			);

			previousTime = currentTime;

			m_window.ProcessEvents();

			if (m_window.ShouldClose())
			{
				break;
			}

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGuiIO& io = ImGui::GetIO();

			const bool focused =
				glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;

			const bool f1Down =
				glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS;

			if (focused && f1Down && !wasF1Down && !io.WantTextInput)
			{
				cameraMode = !cameraMode;

				if (cameraMode)
				{
					m_window.CaptureCursor();
				}
				else
				{
					m_window.ReleaseCursor();
				}
			}

			wasF1Down = f1Down;

			if (!focused && cameraMode)
			{
				cameraMode = false;
				m_window.ReleaseCursor();
			}

			// Inspector
			ImGui::SetNextWindowSize(
				ImVec2(380.0f, 300.0f),
				ImGuiCond_FirstUseEver
			);

			if (ImGui::Begin("Scene Inspector"))
			{
				ImGui::TextUnformatted(
					cameraMode
					? "F1: release mouse to edit"
					: "F1: enable camera controls"
				);

				auto& objects = m_scene.GetObjects();
				const int count = static_cast<int>(objects.size());

				if (count == 0)
				{
					ImGui::TextUnformatted("Scene is empty.");
				}
				else
				{
					if (selectedObject >= count)
					{
						selectedObject = count - 1;
					}

					const std::string name = objects[selectedObject].GetName();

					if (ImGui::BeginCombo("##GameObjectSelector", name.c_str()))
					{
						for (int i = 0; i < count; ++i)
						{
							const std::string label = objects[i].GetName();

							if (ImGui::Selectable(
								label.c_str(), selectedObject == i))
							{
								selectedObject = i;
							}
						}

						ImGui::EndCombo();
					}

					Transform& transform =
						objects[selectedObject].GetTransform();

					glm::vec3 position = transform.GetPosition();
					glm::vec3 rotation = transform.GetRotation();
					glm::vec3 scale = transform.GetScale();

					ImGui::Separator();
					ImGui::TextUnformatted("Components: X / Y / Z");

					if (ImGui::DragFloat3(
						"Position",
						glm::value_ptr(position),
						0.01f))
					{
						transform.SetPosition(position);
					}

					if (ImGui::DragFloat3(
						"Rotation",
						glm::value_ptr(rotation),
						0.5f))
					{
						transform.SetRotation(rotation);
					}

					if (ImGui::DragFloat3(
						"Scale",
						glm::value_ptr(scale),
						0.001f,
						0.001f,
						1000.0f,
						"%.3f",
						ImGuiSliderFlags_AlwaysClamp))
					{
						transform.SetScale(scale);
					}

					ImGui::TextUnformatted("Rotation is in degrees.");
					ImGui::TextUnformatted("Ctrl+click a value to type.");
				}
			}

			ImGui::End();

			// Camera controls
			if (cameraMode && focused)
			{
				const MouseMovement mouse = m_window.GetMouseMovement();

				if (!io.WantCaptureMouse)
				{
					m_camera.Rotate(
						static_cast<float>(mouse.x) * mouseSensitivity,
						static_cast<float>(mouse.y) * -mouseSensitivity
					);
				}

				if (!io.WantCaptureKeyboard)
				{
					glm::vec3 movement(0.0f);

					if (m_window.IsKeyPressed(Key::W)) movement.z += 1.0f;
					if (m_window.IsKeyPressed(Key::S)) movement.z -= 1.0f;
					if (m_window.IsKeyPressed(Key::A)) movement.x -= 1.0f;
					if (m_window.IsKeyPressed(Key::D)) movement.x += 1.0f;
					if (m_window.IsKeyPressed(Key::Q)) movement.y -= 1.0f;
					if (m_window.IsKeyPressed(Key::E)) movement.y += 1.0f;

					if (glm::length(movement) > 0.0f)
					{
						movement = glm::normalize(movement);

						const float distance = movementSpeed * deltaTime;

						m_camera.MoveRelative(
							movement.z * distance,
							movement.x * distance,
							movement.y * distance
						);
					}
				}
			}

			// Draw the scene, then the GUI over it.
			m_renderer.Render(m_window.GetAspectRatio(), m_camera, m_scene);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			m_window.SwapBuffers();
		}
	}
}
