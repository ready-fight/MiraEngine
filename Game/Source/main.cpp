#include "Core/Application.h"
#include "Initialize.h"
#include "Player/Player.h"
#include "Particles/ParticleEmitter.h"

#include <exception>
#include <iostream>

int main()
{
	try {
		MiraEngine::Application application;
		MiraEngine::Camera& camera = application.GetCamera();
		MiraEngine::Scene& scene = application.GetScene();
		InitializeQuests();
		InitializeEnvironment(scene);
		MiraGame::Player& player = InitializePlayer(scene);
		InitializeEnemy(scene, player);
		InitializeInteractions(scene);
		InitializeNPCs(scene);
		//InitializeCollisionObjects(scene);
		InitializeCamera(scene, camera, player);
		application.Run();
	}
	catch (const std::exception& e) {
		std::cerr << "Fatal Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}