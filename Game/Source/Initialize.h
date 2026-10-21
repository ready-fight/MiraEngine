#pragma once
#include <glm/glm.hpp>

namespace MiraEngine
{
    class Scene;
    class Camera;
}

namespace MiraGame
{
    class Door;
    class Player;
}

void InitializeQuests();
void InitializeEnvironment(MiraEngine::Scene& scene);
MiraGame::Player& InitializePlayer(MiraEngine::Scene& scene, const glm::vec3& position);
void InitializeEnemy(MiraEngine::Scene& scene, MiraGame::Player& player);
void InitializeInteractions(MiraEngine::Scene& scene);
MiraGame::Door& InitializeDungeonDoor(MiraEngine::Scene& scene);
void InitializeDungeonEntrance(MiraEngine::Scene& scene, MiraGame::Player& player, const MiraGame::Door& door);
void InitializeNPCs(MiraEngine::Scene& scene);
void InitializeCollisionObjects(MiraEngine::Scene& scene);
void InitializeCamera(MiraEngine::Scene& scene, MiraEngine::Camera& camera, MiraGame::Player& player);
void BuildOverworld(MiraEngine::Scene& scene, MiraEngine::Camera& camera);
void BuildDungeon(MiraEngine::Scene& scene, MiraEngine::Camera& camera);