#include "Application.h"

#include <GL/glew.h>

namespace Wave 
{
	Application::Application() : m_window(1280, 720, "Mira Engine"), m_camera(glm::vec3(1.5f, 1.2f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f))
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (!m_window.ShouldClose())
		{
			m_window.ProcessEvents();

			m_renderer.Render(m_window.GetAspectRatio(), m_camera);

			m_window.SwapBuffers();
		}
	}
}
