#include "Core/Application.h"
#include "Initialize.h"
#include "Player/Player.h"

int main()
{
    MiraEngine::Application application;
    MiraEngine::Camera& camera = application.GetCamera();
	MiraEngine::Scene& scene = application.GetScene();

    InitializeQuests();
    InitializeModels();
    InitializeAnimations();
    MiraGame::Player* player = InitializePlayer(scene);
    InitializeEnemy(scene, player);
    InitializeInteractions(scene);
    InitializeNPCs(scene);
	//InitializeCollisionObjects(scene);
	InitializeCamera(scene, camera, *player);

    application.Run();

    return 0;
}