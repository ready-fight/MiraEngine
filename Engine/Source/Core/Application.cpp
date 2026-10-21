#include "Core/Application.h"
#include "Core/Time.h"
#include "Core/Input.h"

#include "Scene/SceneManager.h"
#include "Scene/GameObject.h"
#include "Scene/Transform.h"
#include "Animation/Animator.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include "UI/UI.h"

#include <algorithm>
#include <string>

#include "Graphics/Model.h"

namespace MiraEngine
{

    namespace
    {
        constexpr float FlyCameraSpeed = 10.0f;
        constexpr float FlyCameraSensitivity = 0.1f;

        // True only on the frame the key goes down, not while it's held.
        bool PressedThisFrame(const Window& window, Key key, bool& wasDown)
        {
            const bool down = window.IsKeyPressed(key);
            const bool pressed = down && !wasDown;
            wasDown = down;
            return pressed;
        }
    }

    Application::Application()
        : m_window(1280, 720, "Mira Engine")
        , m_camera(glm::vec3(0.0f, 3.0f, -5.5f), glm::vec3(0.0f, 1.0f, 0.0f))
    {
        Input::Initialize(m_window, m_camera);
        UI::Initialize(m_window.GetNativeWindow());
        m_window.ReleaseCursor();
    }

    Application::~Application()
    {
        UI::Shutdown();
    }

    void Application::Run()
    {
        m_window.Maximize();

        while (!m_window.ShouldClose())
        {
            Time::Tick();
            const float deltaTime = Time::DeltaTime();
            m_window.ProcessEvents();

            if (m_window.ShouldClose())
            {
                break;
            }

            // Scene changes requested last frame happen here, before anything updates or draws.
            if (SceneManager::ApplyPendingChange(m_scene))
            {
                m_selectedObject = 0;  // the old selection index points at a different object now
            }

            UI::BeginFrame();
            m_scene.DrawUI();

            HandleEditorKeys();
            Input::SetGameplayEnabled(!m_cameraMode);
            m_scene.Update(deltaTime);

            DrawInspector();
            UpdateFlyCamera(deltaTime);

            m_renderer.Render(m_window.GetAspectRatio(), m_camera, m_scene, m_selectedObject ? m_scene.GetObjects()[m_selectedObject].get() : nullptr);
            UI::EndFrame();
            m_window.SwapBuffers();
        }


    }

    Scene& Application::GetScene()
    {
        return m_scene;
    }

    const Scene& Application::GetScene() const
    {
        return m_scene;
    }

    Camera& Application::GetCamera()
    {
        return m_camera;
    }

    const Camera& Application::GetCamera() const
    {
        return m_camera;
    }

    // F1 toggles the fly camera, F2 toggles collider wireframes.
    void Application::HandleEditorKeys()
    {
        const bool focused = m_window.IsFocused();
        const bool typing = ImGui::GetIO().WantTextInput;

        // Always poll both keys so the "was down" state stays current.
        const bool f1 = PressedThisFrame(m_window, Key::F1, m_wasF1Down);
        const bool f2 = PressedThisFrame(m_window, Key::F2, m_wasF2Down);
        const bool f5 = PressedThisFrame(m_window, Key::F5, m_wasF5Down);

        if (focused && !typing && f1)
        {
            SetCameraMode(!m_cameraMode);
        }

        if (focused && !typing && f2)
        {
            m_renderer.SetDebugColliders(!m_renderer.GetDebugColliders());
        }

        if (focused && !typing && f5)
        {
            SceneManager::RequestChange(SceneManager::GetCurrentName());
        }

        if (!focused && m_cameraMode)
        {
            SetCameraMode(false);
        }
    }

    void Application::SetCameraMode(bool enabled)
    {
        m_cameraMode = enabled;

        if (enabled)
        {
            m_window.CaptureCursor();
        }
        else
        {
            m_window.ReleaseCursor();
        }
    }

    void Application::DrawInspector()
    {
        ImGui::SetNextWindowSize(ImVec2(380.0f, 300.0f), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Scene Inspector"))
        {
            DrawInspectorContents();
        }

        ImGui::End();
    }

    void Application::DrawInspectorContents()
    {
        auto& objects = m_scene.GetObjects();
        const int count = static_cast<int>(objects.size());

        if (count == 0)
        {
            ImGui::TextUnformatted("Scene is empty.");
            return;
        }

        ImGui::TextUnformatted("F1: Camera Mode");
        ImGui::TextUnformatted("F2: Show Collision");
        ImGui::TextUnformatted("F5: Reload Scene");
        ImGui::TextUnformatted("F: Interact");
        ImGui::TextUnformatted("E: Attack");
        ImGui::Separator();

        m_selectedObject = std::min(m_selectedObject, count - 1);

        if (ImGui::BeginCombo("##GameObjectSelector", objects[m_selectedObject]->GetName().c_str()))
        {

            for (int i = 0; i < count; ++i)
            {

                const std::string& label = objects[i]->GetName();

                if (label == "CameraController")
                {
                    continue;
                }

                if (ImGui::Selectable(label.c_str(), m_selectedObject == i))
                {
                    m_selectedObject = i;
                }
            }

            ImGui::EndCombo();
        }

        GameObject& selected = *objects[m_selectedObject];
        Transform& transform = selected.GetTransform();
        glm::vec3 position = transform.GetPosition();
        glm::vec3 rotation = transform.GetRotation();
        glm::vec3 scale = transform.GetScale();

        ImGui::Separator();
        ImGui::TextUnformatted("Transform");
        ImGui::Separator();

        if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.01f))
        {
            transform.SetPosition(position);
        }

        if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.5f))
        {
            transform.SetRotation(rotation);
        }

        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.001f, 0.001f, 1000.0f, "%.3f",
            ImGuiSliderFlags_AlwaysClamp))
        {
            transform.SetScale(scale);
        }

        ImGui::Separator();

        if (Collider* collider = selected.GetCollider())
        {
            ImGui::TextUnformatted("Collider");
            ImGui::Separator();

            if (collider->shape == Collider::Shape::Sphere)
            {
                ImGui::DragFloat("Radius", &collider->radius, 0.1f);
            }
            else
            {
                ImGui::DragFloat3("Half Extents", glm::value_ptr(collider->halfExtents), 0.1f);
            }
        }
    }

    void Application::UpdateFlyCamera(float deltaTime)
    {
        if (!m_cameraMode)
        {
            return;
        }

        const ImGuiIO& io = ImGui::GetIO();
        const MouseMovement mouse = m_window.GetMouseMovement();

        if (!io.WantCaptureMouse)
        {
            m_camera.Rotate(mouse.x * FlyCameraSensitivity, -mouse.y * FlyCameraSensitivity);
        }

        if (io.WantCaptureKeyboard)
        {
            return;
        }

        glm::vec3 movement(0.0f);

        if (m_window.IsKeyPressed(Key::W))
        {
            movement.z += 1.0f;
        }
        if (m_window.IsKeyPressed(Key::S))
        {
            movement.z -= 1.0f;
        }
        if (m_window.IsKeyPressed(Key::A))
        {
            movement.x -= 1.0f;
        }
        if (m_window.IsKeyPressed(Key::D))
        {
            movement.x += 1.0f;
        }
        if (m_window.IsKeyPressed(Key::Q))
        {
            movement.y -= 1.0f;
        }
        if (m_window.IsKeyPressed(Key::E))
        {
            movement.y += 1.0f;
        }

        if (glm::length(movement) > 0.0f)
        {
            movement = glm::normalize(movement) * FlyCameraSpeed * deltaTime;
            m_camera.MoveRelative(movement.z, movement.x, movement.y);
        }
    }
}
