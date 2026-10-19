#include "Core/Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace
{
	int ToGlfwKey(MiraEngine::Key key)
	{
		using MiraEngine::Key;
		switch (key)
		{
		case Key::W:  return GLFW_KEY_W;
		case Key::A:  return GLFW_KEY_A;
		case Key::S:  return GLFW_KEY_S;
		case Key::D:  return GLFW_KEY_D;
		case Key::Q:  return GLFW_KEY_Q;
		case Key::E:  return GLFW_KEY_E;
		case Key::F:  return GLFW_KEY_F;
		case Key::I:  return GLFW_KEY_I;
		case Key::F1: return GLFW_KEY_F1;
		case Key::F2: return GLFW_KEY_F2;
		}
		return GLFW_KEY_UNKNOWN;
	}

	int ToGlfwMouseButton(MiraEngine::MouseButton button)
	{
		using MiraEngine::MouseButton;
		switch (button)
		{
		case MouseButton::Left:   return GLFW_MOUSE_BUTTON_LEFT;
		case MouseButton::Right:  return GLFW_MOUSE_BUTTON_RIGHT;
		case MouseButton::Middle: return GLFW_MOUSE_BUTTON_MIDDLE;
		}
		return GLFW_MOUSE_BUTTON_LEFT;
	}
}

namespace MiraEngine
{
	Window::Window(int width, int height, const char* title)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

		if (!m_window)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create window");
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

		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int newWidth, int newHeight)
			{ glViewport(0, 0, newWidth, newHeight); });

		int framebufferWidth;
		int framebufferHeight;
		glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferHeight);
		glfwSetWindowUserPointer(m_window, this);

		glfwSetScrollCallback(m_window,
			[](GLFWwindow* window, double xOffset, double yOffset)
			{
				Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

				if (self)
				{
					self->m_mouseScrollY += static_cast<float>(yOffset);
				}
			});
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
		m_mouseScrollY = 0.0f;
		glfwPollEvents();

		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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
		return glfwGetKey(m_window, ToGlfwKey(key)) == GLFW_PRESS;
	}

	bool Window::IsMouseButtonPressed(MouseButton button) const
	{
		return glfwGetMouseButton(m_window, ToGlfwMouseButton(button)) == GLFW_PRESS;
	}

	bool Window::IsFocused() const
	{
		return glfwGetWindowAttrib(m_window, GLFW_FOCUSED) == GLFW_TRUE;
	}

	void Window::Maximize()
	{
		glfwMaximizeWindow(m_window);
	}

	float Window::GetMouseScroll() const
	{
		return m_mouseScrollY;
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

		MouseMovement movement{ static_cast<float>(currentX - m_lastMouseX), static_cast<float>(currentY - m_lastMouseY) };
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
