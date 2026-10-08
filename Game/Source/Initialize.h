#pragma once


namespace MiraEngine {
	class Scene;
}

namespace MiraGame
{
	class Player;
}

void InitializeQuests();
MiraGame::Player* InitializePlayer(MiraEngine::Scene& scene);
void InitializeEnemy(MiraEngine::Scene& scene, MiraGame::Player* playerPtr);
void InitializeInteractions(MiraEngine::Scene& scene);
void InitializeNPCs(MiraEngine::Scene& scene);
void InitializeCollisionObjects(MiraEngine::Scene& scene);