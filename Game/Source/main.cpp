#include "Core/Application.h"
#include "Initialize.h"

int main()
{
    MiraEngine::Application application;

    InitializeQuests();

	InitializePlayer_Enemy(application.GetScene());

    InitializeInteractions(application.GetScene());

    InitializeNPCs(application.GetScene());

	InitializeCollisionObjects(application.GetScene());

    application.Run();

    return 0;
}