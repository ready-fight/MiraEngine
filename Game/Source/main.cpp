#include "Core/Application.h"
#include "Scene/SceneManager.h"
#include "Initialize.h"

#include <exception>
#include <iostream>

int main()
{
    try
    {
        MiraEngine::Application application;
        MiraEngine::Camera& camera = application.GetCamera();

        InitializeQuests();

        MiraEngine::SceneManager::Register("Overworld",
            [&camera](MiraEngine::Scene& scene) { BuildOverworld(scene, camera); });
        MiraEngine::SceneManager::RequestChange("Overworld");

        application.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}