#include "Core/Application.h"
#include "Initialize.h"
#include "Player/Player.h"

int main()
{
    MiraEngine::Application application;
	MiraEngine::Scene& scene = application.GetScene();
	MiraGame::Player* player = InitializePlayer(scene);

    InitializeQuests();
    InitializeEnemy(scene, player);
    InitializeInteractions(scene);
    InitializeNPCs(scene);
	InitializeCollisionObjects(scene);

    application.SetCameraTarget(player);
    application.Run();

    return 0;
}