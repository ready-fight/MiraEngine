#pragma once

#include "Graphics/Camera.h"
#include "Graphics/Renderer.h"
#include "Core/Window.h"
#include "Scene/Scene.h"

namespace MiraEngine
{

    class GameObject;

    class Application
    {
    public:
        Application();
        ~Application();
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        void Run();
        Scene& GetScene();
        const Scene& GetScene() const;
        Camera& GetCamera();
        const Camera& GetCamera() const;

    private:
        void HandleEditorKeys();
        void SetCameraMode(bool enabled);
        void DrawInspector();
        void DrawInspectorContents();
        void UpdateFlyCamera(float deltaTime);

        Window m_window;
        Camera m_camera;
        Renderer m_renderer;
        Scene m_scene;

        int m_selectedObject = 0;
        bool m_cameraMode = false;
        bool m_wasF1Down = false;
        bool m_wasF2Down = false;
    };
}
