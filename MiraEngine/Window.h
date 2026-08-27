#pragma once

struct GLFWwindow;

namespace Wave {

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

	private:
		GLFWwindow* m_window = nullptr;
	};
}

