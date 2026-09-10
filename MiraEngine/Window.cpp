#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Mira {
	Window::Window(int width, int height, const char* title)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(
			width,
			height,
			title,
			nullptr,
			nullptr
		);

		if (!m_window)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to initialize GLFW");
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);

		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();

			throw std::runtime_error("Failed to initialize GLEW");
		}


		glfwSetFramebufferSizeCallback(m_window,
			[](GLFWwindow* window, int newWidth, int newHeight)
			{
				glViewport(0, 0, newWidth, newHeight);
			}
		);

		int framebufferWidth;
		int framebufferHeight;

		glfwGetFramebufferSize(
			m_window,
			&framebufferWidth,
			&framebufferHeight
		);

		glViewport(0, 0, framebufferWidth, framebufferHeight);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

	void Window::ProcessEvents()
	{
		glfwPollEvents();

		if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(m_window, true);
		}
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	float Window::GetAspectRatio() const
	{
		int width;
		int height;

		glfwGetFramebufferSize(m_window, &width, &height);

		if (height == 0)
		{
			return 1.0f;
		}

		return static_cast<float>(width) / static_cast<float>(height);
	}

	bool Window::IsKeyPressed(Key key) const
	{
		int glfwKey = GLFW_KEY_UNKNOWN;

		switch (key) {
		case Key::W:
			glfwKey = GLFW_KEY_W;
			break;
		case Key::A:
			glfwKey = GLFW_KEY_A;
			break;

		case Key::S:
			glfwKey = GLFW_KEY_S;
			break;

		case Key::D:
			glfwKey = GLFW_KEY_D;
			break;

		case Key::Q:
			glfwKey = GLFW_KEY_Q;
			break;

		case Key::E:
			glfwKey = GLFW_KEY_E;
			break;
		}

		return glfwGetKey(m_window, glfwKey) == GLFW_PRESS;
	}
	void Window::CaptureCursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_firstMouseMovement = true;
	}

	MouseMovement Window::GetMouseMovement()
	{
		double currentX;
		double currentY;

		glfwGetCursorPos(m_window, &currentX, &currentY);

		if (m_firstMouseMovement)
		{
			m_lastMouseX = currentX;
			m_lastMouseY = currentY;
			m_firstMouseMovement = false;

			return {};
		}

		MouseMovement movement;
		
		movement.x = currentX - m_lastMouseX;
		movement.y = currentY - m_lastMouseY;

		m_lastMouseX = currentX;
		m_lastMouseY = currentY;

		return movement;

	}
	GLFWwindow* Window::GetNativeWindow() const
	{
		return m_window;
	}

	void Window::ReleaseCursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_firstMouseMovement = true;
	}
}
