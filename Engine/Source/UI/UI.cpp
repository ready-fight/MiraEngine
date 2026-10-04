#include "UI/UI.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>

namespace MiraEngine
{
    void UI::Initialize(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        {
            ImGui::DestroyContext();

            throw std::runtime_error(
                "Failed to initialize ImGui GLFW"
            );
        }

        if (!ImGui_ImplOpenGL3_Init("#version 460 core"))
        {
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            throw std::runtime_error(
                "Failed to initialize ImGui OpenGL"
            );
        }
    }

    void UI::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UI::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void UI::EndFrame()
    {
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(
            ImGui::GetDrawData()
        );
    }

    bool UI::BeginWindow(const std::string& title)
    {
        return ImGui::Begin(title.c_str());
    }

    void UI::EndWindow()
    {
        ImGui::End();
    }

    void UI::DrawText(const std::string& text)
    {
        ImGui::TextUnformatted(text.c_str());
    }

    bool UI::DrawButton(const std::string& label)
    {
        return ImGui::Button(label.c_str());
    }

    void UI::DrawProgressBar(
        float value,
        const std::string& text
    )
    {
        ImGui::ProgressBar(
            value,
            ImVec2(250.0f, 20.0f),
            text.c_str()
        );
    }
}