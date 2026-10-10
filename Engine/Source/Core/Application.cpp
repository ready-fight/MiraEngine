#include "Core/Application.h"
#include "Core/Time.h"
#include "Core/Input.h"

#include "Scene/GameObject.h"
#include "Scene/Transform.h"
#include "Animation/Animator.h"
#include "Collision/SphereCollider.h"
#include "Collision/BoxCollider.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <imgui.h>
#include "UI/UI.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>
#include <memory>

#include <cmath>

#include "Graphics/Model.h"


namespace MiraEngine 
{
	Application::Application() : m_window(1280, 720, "Mira Engine"), m_camera(
			glm::vec3(0.0f, 3.0f, -5.5f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		)
	{

		Input::Initialize(m_window, m_camera);

		UI::Initialize(m_window.GetNativeWindow());

		m_window.ReleaseCursor();
	}

	Application::~Application()
	{
		UI::Shutdown();
	}

	void Application::Run()
	{
		const float movementSpeed = 10.0f;
		const float mouseSensitivity = 0.1f;

		int selectedObject = 0;

		bool cameraMode = false;

		bool wasF1Down = false;
		bool wasF2Down = false;

		bool debugColliders = false;

		GLFWwindow* window = m_window.GetNativeWindow();

		if (!glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
		{
			glfwMaximizeWindow(window);
		}

		while (!m_window.ShouldClose())
		{
			Time::Tick();

			const float deltaTime = Time::DeltaTime();

			m_window.ProcessEvents();


			if (m_window.ShouldClose())
			{
				break;
			}

			UI::BeginFrame();

			m_scene.DrawUI();

			ImGuiIO& io = ImGui::GetIO();

			const bool focused =
				glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;

			// F1
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

			// F2
			const bool f2Down =
				glfwGetKey(
					window,
					GLFW_KEY_F2
				) == GLFW_PRESS;

			if (
				focused &&
				f2Down &&
				!wasF2Down &&
				!io.WantTextInput
				)
			{
				debugColliders =
					!debugColliders;

				m_renderer.SetDebugColliders(
					debugColliders
				);
			}

			wasF2Down = f2Down;

			Input::SetGameplayEnabled(
				!cameraMode
			);

			m_scene.Update(deltaTime);

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
				/*ImGui::TextUnformatted(
					cameraMode
					? "F1: release mouse to edit"
					: "F1: enable camera controls"
				);*/

				auto& objects = m_scene.GetObjects();
				const int count = static_cast<int>(objects.size());

				if (count == 0)
				{
					ImGui::TextUnformatted("Scene is empty.");
				}
				else
				{

					ImGui::TextUnformatted("F1: Camera Mode");
					ImGui::TextUnformatted("F2: Show Collision");
					ImGui::TextUnformatted("F: Interact");
					ImGui::TextUnformatted("E: Attack");

					ImGui::Separator();

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

							if (label == "CameraController") continue;

							std::cout << label << "\n";

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

					/*if (ImGui::DragFloat(
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
					}*/

					ImGui::Separator();

					ImGui::TextUnformatted("Transform");

					ImGui::Separator();

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

					ImGui::Separator();

					Collider* collider = objects[selectedObject]->GetCollider();

					if (collider) {
						ImGui::TextUnformatted("Collider");
						ImGui::Separator();

						if (
							auto* sphere =
							dynamic_cast<
							SphereCollider*
							>(collider)
							)
						{
							float radius = sphere->GetRadius();

							if (ImGui::DragFloat(
								"Radius",
								&radius,
								0.1f))
							{
								sphere->SetRadius(radius);
							}
						}
						else if (
							auto* box =
							dynamic_cast<
							BoxCollider*
							>(collider)
							)
						{
							glm::vec3 halfExtents = box->GetHalfExtents();

							if (ImGui::DragFloat3(
								"Half Extents",
								glm::value_ptr(halfExtents),
								0.1f))
							{
								box->SetHalfExtents(halfExtents);
							}
						}
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

			UI::EndFrame();

			m_window.SwapBuffers();
		}
	}

	Scene& Application::GetScene()
	{
		return m_scene;
	}

	const Scene& Application::GetScene() const
	{
		return m_scene;
	}

	Camera& Application::GetCamera()
	{
		return m_camera;
	}

	const Camera& Application::GetCamera() const
	{
		return m_camera;
	}
}
