#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "Window.h"
#include "Scene.h"

namespace Mira
{
	class Application
	{
		public:
			Application();
			~Application();

			Application(const Application&) = delete;
			Application& operator=(const Application&) = delete;

			void Run();

		private:
			Window m_window;
			Camera m_camera;
			Renderer m_renderer;
			Scene m_scene;
	};
}

