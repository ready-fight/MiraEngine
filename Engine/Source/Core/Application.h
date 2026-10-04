#pragma once

#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Core/Window.h"
#include "Scene/Scene.h"

namespace MiraEngine
{
	class Application
	{
		public:
			Application();
			~Application();

			Application(const Application&) = delete;
			Application& operator=(const Application&) = delete;

			void Run();

			Scene& GetScene();
			const Scene& GetScene() const;

		private:
			Window m_window;
			Camera m_camera;
			Renderer m_renderer;
			Scene m_scene;
	};
}

