#pragma once

#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Core/Window.h"
#include "Scene/Scene.h"

namespace MiraEngine
{

	class GameObject;

	class Application
	{
		public:
			Application();
			~Application();
			Application(const Application&) = delete;
			Application& operator=(const Application&) = delete;

			void Run();
			void SetCameraTarget(GameObject* target);
			Scene& GetScene();
			const Scene& GetScene() const;

		private:
			Window m_window;
			Camera m_camera;
			Renderer m_renderer;
			Scene m_scene;
			GameObject* m_cameraTarget = nullptr;

			glm::vec3 m_gameplayCameraOffset =
				glm::vec3(
					0.0f,
					3.0f,
					-5.5f
				);

			glm::vec3 m_gameplayCameraLookOffset =
				glm::vec3(
					0.0f,
					1.0f,
					0.0f
				);
	};
}

