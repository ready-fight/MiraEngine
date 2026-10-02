#pragma once

#include <string>

struct GLFWwindow;

namespace Mira
{
    class UI
    {
    public:
        static void Initialize(GLFWwindow* window);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        static bool BeginWindow(const std::string& title);
        static void EndWindow();

        static void DrawText(const std::string& text);
        static bool DrawButton(const std::string& label);
    };
}