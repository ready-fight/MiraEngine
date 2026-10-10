#pragma once

namespace MiraEngine {
	class Scene;
	class Camera;
}

namespace MiraGame
{
	class Player;
}

void InitializeQuests();
void InitializeModels();
void InitializeAnimations();
MiraGame::Player* InitializePlayer(MiraEngine::Scene& scene);
void InitializeEnemy(MiraEngine::Scene& scene, MiraGame::Player* playerPtr);
void InitializeInteractions(MiraEngine::Scene& scene);
void InitializeNPCs(MiraEngine::Scene& scene);
void InitializeCollisionObjects(MiraEngine::Scene& scene);
void InitializeCamera(MiraEngine::Scene& scene, MiraEngine::Camera& camera, MiraGame::Player& player);