#pragma once

#include "Camera.h"
#include "Renderer.h"
#include "Window.h"

namespace Wave 
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
	};
}

