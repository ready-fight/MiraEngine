#pragma once

#include <string>

struct GLFWwindow;

namespace MiraEngine
{
    class UI
    {
    public:
        static void Initialize(GLFWwindow* window);
        static void Shutdown();
        static void BeginFrame();
        static void EndFrame();
        static bool BeginWindow(const std::string& title, bool* open = nullptr, bool autoResize = false);
        static bool DrawSlot(const std::string& label, float size, bool highlighted);
        static void DrawSeparator();
        static void SameLine();
        static void EndWindow();
        static bool DrawButton(const std::string& label);
        static void DrawText(const std::string& text);
        static void DrawProgressBar(float value, const std::string& text);
        static void PushID(int id);
        static void PopID();
    };
}