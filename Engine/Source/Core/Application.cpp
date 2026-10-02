#include "Core/Application.h"

#include "Collision/SphereCollider.h"

#include "Scene/GameObject.h"
#include "Scene/Transform.h"

#include <glm/vec3.hpp>

#include "Core/Time.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>


namespace Mira 
{
	Application::Application() : m_window(1280, 720, "Mira Engine"), m_camera(
			glm::vec3(-0.0f, 2.0f, -5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f)
		)
	{
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
		auto objectA = std::make_unique<GameObject>("Object A");
		auto objectB = std::make_unique<GameObject>("Object B");

		objectA->GetTransform().SetPosition(
			glm::vec3(0.0f, 0.0f, 0.0f)
		);

		objectB->GetTransform().SetPosition(
			glm::vec3(1.2f, 0.0f, 0.0f)
		);

		objectA->SetCollider(
			std::make_unique<SphereCollider>(0.5f)
		);

		objectB->SetCollider(
			std::make_unique<SphereCollider>(0.5f)
		);

		GameObject* a = objectA.get();

		m_scene.AddObject(std::move(objectA));
		m_scene.AddObject(std::move(objectB));

		// Try moving A toward B
		const glm::vec3 oldPosition =
			a->GetTransform().GetPosition();

		a->GetTransform().SetPosition(
			oldPosition + glm::vec3(0.3f, 0.0f, 0.0f)
		);

		if (m_scene.IsColliding(*a))
		{
			std::cout << "Collision detected - undoing movement\n";

			a->GetTransform().SetPosition(oldPosition);
		}

		const float movementSpeed = 2.0f;
		const float mouseSensitivity = 0.1f;

		int selectedObject = 0;
		bool cameraMode = false;
		bool wasF1Down = false;

		GLFWwindow* window = m_window.GetNativeWindow();

		while (!m_window.ShouldClose())
		{
			Time::Tick();

			const float deltaTime = Time::DeltaTime();

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

					const std::string name = objects[selectedObject]->GetName();

					if (ImGui::BeginCombo("##GameObjectSelector", name.c_str()))
					{
						for (int i = 0; i < count; ++i)
						{
							const std::string label = objects[i]->GetName();

							if (ImGui::Selectable(
								label.c_str(), selectedObject == i))
							{
								selectedObject = i;
							}
						}

						ImGui::EndCombo();
					}

					Transform& transform =
						objects[selectedObject]->GetTransform();

					glm::vec3 position = transform.GetPosition();
					glm::vec3 rotation = transform.GetRotation();
					glm::vec3 scale = transform.GetScale();
					float yaw = m_camera.GetYaw();
					float pitch = m_camera.GetPitch();

					ImGui::Separator();
					ImGui::TextUnformatted("Components: X / Y / Z");

					if (ImGui::DragFloat(
						"Pitch",
						&pitch,
						0.01f))
					{
						m_camera.SetPitch(pitch);
					}

					if (ImGui::DragFloat(
						"Yaw",
						&yaw,
						0.01f))
					{
						m_camera.SetYaw(yaw);
					}

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

				}
			}

			ImGui::End();

			glm::vec3 movement(0.0f);

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
