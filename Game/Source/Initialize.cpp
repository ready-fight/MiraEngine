#include "Initialize.h"

#include "Player/Player.h"
#include "Camera/CameraController.h"
#include "Graphics/Camera.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"
#include "Collision/BoxCollider.h"
#include "Quest/QuestDatabase.h"
#include "Interaction/Chest.h"
#include "Interaction/Door.h"
#include "Interaction/NPC.h"
#include "Assets/AssetManager.h"
#include "Graphics/Model.h"
#include "Scene/Scene.h"

#include <glm/vec3.hpp>
#include <memory>

void InitializeQuests() {
    MiraGame::Quest dungeonKeyQuest;

    dungeonKeyQuest.name =
        "Find the Dungeon Key";

    dungeonKeyQuest.description =
        "Find the key that opens the dungeon door.";

    dungeonKeyQuest.requiredItem =
        "Dungeon Key";

    MiraGame::QuestDatabase::RegisterQuest(
        dungeonKeyQuest
    );
}

void InitializeModels() {
    MiraEngine::AssetManager::LoadModel("Assets/Models/Idle.fbx");
}

void InitializeAnimations() {
    MiraEngine::AssetManager::LoadAnimation("Assets/Models/Idle.fbx");
    MiraEngine::AssetManager::LoadAnimation("Assets/Models/Walking.fbx");
    MiraEngine::AssetManager::LoadAnimation("Assets/Models/Running.fbx");
    MiraEngine::AssetManager::LoadAnimation("Assets/Models/Attack.fbx");
    MiraEngine::AssetManager::LoadAnimation("Assets/Models/Death.fbx");
    MiraEngine::AssetManager::LoadAnimation("Assets/Models/Interactable/NPC.fbx");
}

MiraGame::Player* InitializePlayer(MiraEngine::Scene& scene) {
    auto player =
        std::make_unique<MiraGame::Player>(
            &scene
        );

    MiraGame::Player* playerPtr =
        player.get();


    player->GetTransform().SetPosition(
        glm::vec3(0.886f, 0.0f, -3.98f)
    );

    scene.AddObject(
        std::move(player)
    );

	return playerPtr;
}

void InitializeEnemy(MiraEngine::Scene& scene, MiraGame::Player* playerPtr) {
    auto enemy =
        std::make_unique<MiraGame::Enemy>(
            playerPtr,
            &scene
        );

    enemy->GetMaterial().color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    enemy->GetTransform().SetPosition(
        glm::vec3(5.0f, 0.0f, 5.0f)
    );

    scene.AddObject(
        std::move(enemy)
    );
}

void InitializeInteractions(MiraEngine::Scene& scene) {
    // Sword
    MiraGame::ItemData swordData;
    swordData.name = "Sword";
    swordData.type = MiraGame::ItemType::Weapon;
    swordData.damage = 20;

    auto item =
        std::make_unique<MiraGame::Item>(
            swordData
        );

    auto swordModel =
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Interactable/Sword/Sword.obj"
        );

    item->SetModel(swordModel);

    item->GetTransform().SetRotation(
        glm::vec3(-90.f, 0.f, 0.f)
    );

    item->GetTransform().SetScale(
        glm::vec3(0.1f)
    );

    item->GetTransform().SetPosition(
        glm::vec3(2.0f, 0.0f, 0.0f)
    );

    scene.AddObject(
        std::move(item)
    );

    // Potion
    MiraGame::ItemData potionData;
    potionData.name = "Potion";
    potionData.type = MiraGame::ItemType::Consumable;
    potionData.healAmount = 30;

    auto potion =
        std::make_unique<MiraGame::Item>(
            potionData
        );

    potion->GetTransform().SetScale(
        glm::vec3(0.06f)
    );

    auto potionModel =
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Interactable/Potion/potion.obj"
        );

    potion->SetModel(potionModel);

    potion->GetTransform().SetPosition(
        glm::vec3(-2.0f, 0.0f, 3.0f)
    );

    scene.AddObject(
        std::move(potion)
    );

    // Dungeon Key
    MiraGame::ItemData dungeonKeyData;
    dungeonKeyData.name = "Dungeon Key";
    dungeonKeyData.type = MiraGame::ItemType::KeyItem;

    auto dungeonKey =
        std::make_unique<MiraGame::Item>(
            dungeonKeyData
        );

    dungeonKey->GetMaterial().color.a = 0.0f;

    dungeonKey->GetTransform().SetScale(
        glm::vec3(0.5f)
    );

    auto dungeonKeyModel =
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Interactable/Key/Key.fbx"
        );

    dungeonKey->SetModel(dungeonKeyModel);

    dungeonKey->GetTransform().SetPosition(
        glm::vec3(-1.68f, 0.0f, 1.0f)
    );

    scene.AddObject(
        std::move(dungeonKey)
    );

    // Chest
    MiraGame::ItemData chestPotion;
    chestPotion.name = "Chest Potion";
    chestPotion.type =
        MiraGame::ItemType::Consumable;
    chestPotion.healAmount = 50;

    auto chest =
        std::make_unique<MiraGame::Chest>(chestPotion);

    auto chestModel =
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Interactable/Chest/Chest.obj"
        );

    chest->SetModel(chestModel);

    chest->GetTransform().SetPosition(
        glm::vec3(1.07f, 0.0f, 12.0f)
    );

    chest->GetTransform().SetRotation(
        glm::vec3(0.0f, 90.0f, 0.0f)
    );

    chest->GetTransform().SetScale(
        glm::vec3(2.0f, 2.0f, 2.0f)
    );

    scene.AddObject(
        std::move(chest)
    );

    // Door
    auto door =
        std::make_unique<MiraGame::Door>("Dungeon Key");

    auto doorModel =
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Interactable/Door/Door.fbx"
        );

    door->SetModel(doorModel);

    door->GetTransform().SetPosition(
        glm::vec3(1.5f, 0.0f, 3.0f)
    );

    door->GetTransform().SetRotation(
        glm::vec3(0.0f, 90.0f, 0.0f)
    );

    door->GetTransform().SetScale(
        glm::vec3(0.005f, 0.005f, 0.005f)
    );

    scene.AddObject(
        std::move(door)
    );
}

void InitializeNPCs(MiraEngine::Scene& scene) {
    auto npc =
        std::make_unique<MiraGame::NPC>(
            "Village Elder",

            // Quest not started
            std::vector<std::string>{
        "Greetings, traveler.",
            "The dungeon lies beyond the locked door.",
            "Find the Dungeon Key and you may enter."
    },

            // Quest active
            std::vector<std::string>{
        "Have you found the Dungeon Key yet?",
            "Search carefully. It must be nearby."
    },

            // Quest completed
            std::vector<std::string>{
        "Excellent. You found the Dungeon Key.",
            "The dungeon awaits you."
    },

            // Quest turned in
            std::vector<std::string>{
        "Good luck in the dungeon."
    },

            "Find the Dungeon Key"
        );

    npc->GetMaterial().color = glm::vec4(0, .5, 1, 1);

    npc->GetTransform().SetPosition(
        glm::vec3(-3.3f, 0.0f, 2.0f)
    );

    npc->GetTransform().SetRotation(
        glm::vec3(0.f, 120.f, 0.f)
    );

    scene.AddObject(
        std::move(npc)
    );
}

void InitializeCollisionObjects(MiraEngine::Scene& scene) {

    auto wallModel =
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Collision/Wall/Wall.obj"
        );

    auto wall1 =
        std::make_unique<MiraEngine::GameObject>(
            "Wall 1"
        );

    wall1->SetModel(wallModel);

    wall1->SetCollider(
        std::make_unique<MiraEngine::BoxCollider>(
            glm::vec3(2.95f, 0.7f, 0.15f)
        )
    );

    wall1->GetTransform().SetPosition(
        glm::vec3(0.0f, 0.0f, 10.0f)
    );

    wall1->GetTransform().SetRotation(
        glm::vec3(0.0f, -90.0f, -5.0f)
    );

    scene.AddObject(
        std::move(wall1)
    );
    auto wall2 =
        std::make_unique<MiraEngine::GameObject>(
            "Wall 2"
        );

    wall2->SetModel(wallModel);

    /*wall2->SetDiffuseOverride(
        "Assets/Models/Collision/Wall/tex_u1_v1_normal.jpg"
    );*/

    wall2->SetCollider(
        std::make_unique<MiraEngine::BoxCollider>(
            glm::vec3(2.95f, 0.7f, 0.15f)
        )
    );

    wall2->GetTransform().SetPosition(
        glm::vec3(-6.0f, 0.0f, 10.0f)
    );

    wall2->GetTransform().SetRotation(
        glm::vec3(0.0f, -90.0f, 0.0f)
    );

    scene.AddObject(
        std::move(wall2)
    );
}

void InitializeCamera(
    MiraEngine::Scene& scene,
    MiraEngine::Camera& camera,
    MiraGame::Player& player
)
{
    auto cameraController =
        std::make_unique<
        MiraGame::CameraController
        >(
            camera,
            player
        );

    scene.AddObject(
        std::move(
            cameraController
        )
    );
}