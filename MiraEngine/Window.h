#pragma once

struct GLFWwindow;

namespace Mira {

	struct MouseMovement {
		double x = 0.0f;
		double y = 0.0f;
	};

	enum class Key {
		W,
		A,
		S,
		D,
		Q,
		E
	};

	class Window
	{
		public:
			Window(int width, int height, const char* title);
			~Window();

			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;

			bool ShouldClose() const;
			void ProcessEvents();
			void SwapBuffers();
			float GetAspectRatio() const;
			bool IsKeyPressed(Key key) const;
			void CaptureCursor();
			MouseMovement GetMouseMovement();
			GLFWwindow* GetNativeWindow() const;
			void ReleaseCursor();

		private:
			GLFWwindow* m_window = nullptr;
			double m_lastMouseX = 0.0;
			double m_lastMouseY = 0.0;
			bool m_firstMouseMovement = true;
	};
}

