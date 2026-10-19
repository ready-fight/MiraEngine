#pragma once

struct GLFWwindow;

namespace MiraEngine
{

    struct MouseMovement
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    enum class Key
    {
        W, A, S, D, Q, E, F, I,
        F1, F2
    };

    enum class MouseButton
    {
        Left,
        Right,
        Middle
    };

    class Window
    {
    public:
        Window(int width, int height, const char* title);
        ~Window();
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        bool ShouldClose() const;
        bool IsMouseButtonPressed(MouseButton button) const;
        bool IsKeyPressed(Key key) const;
        void ProcessEvents();
        void SwapBuffers();
        void CaptureCursor();
        void ReleaseCursor();
        float GetAspectRatio() const;
        bool IsFocused() const;
        void Maximize();
        float GetMouseScroll() const;
        MouseMovement GetMouseMovement();
        GLFWwindow* GetNativeWindow() const;

    private:
        GLFWwindow* m_window = nullptr;
        double m_lastMouseX = 0.0;
        double m_lastMouseY = 0.0;
        float m_mouseScrollY = 0.0f;
        bool m_firstMouseMovement = true;
    };
}
