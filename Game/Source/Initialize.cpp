#include "Initialize.h"

#include "Player/Player.h"
#include "Camera/CameraController.h"
#include "Graphics/Camera.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"
#include "Collision/Collider.h"
#include "Quest/QuestDatabase.h"
#include "Interaction/Chest.h"
#include "Interaction/Door.h"
#include "Interaction/NPC.h"
#include "Assets/AssetManager.h"
#include "Graphics/Model.h"
#include "Scene/Scene.h"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <memory>
#include <random>
#include <string>
#include <vector>

void InitializeQuests()
{
    MiraGame::Quest dungeonKeyQuest;
    dungeonKeyQuest.name = "Find the Dungeon Key";
    dungeonKeyQuest.description = "Find the key that opens the dungeon door.";
    dungeonKeyQuest.requiredItem = "Dungeon Key";
    MiraGame::QuestDatabase::RegisterQuest(dungeonKeyQuest);
}

void InitializeModels_Animations()
{
    auto model = MiraEngine::AssetManager::LoadModel("Assets/Models/Idle.fbx");
    auto idle = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Idle.fbx");
    auto walking = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Walking.fbx");
    auto running = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Running.fbx");
    auto attack = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Attack.fbx");
    auto death = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Death.fbx");
    auto npc = MiraEngine::AssetManager::LoadAnimation("Assets/Models/Interactable/NPC.fbx");
    model->AddAnimation(idle);
    model->AddAnimation(walking);
    model->AddAnimation(running);
    model->AddAnimation(attack);
    model->AddAnimation(death);
    model->AddAnimation(npc);
}

void InitializeEnvironment(MiraEngine::Scene& scene)
{
    // Ground: a 40x40 plane with a tiling PBR material (ambientCG Ground037, CC0)
    const std::string groundTextures = "Assets/Textures/Ground/Ground037_1K-JPG_";

    auto ground = std::make_unique<MiraEngine::GameObject>("Ground");
    ground->SetModel(MiraEngine::AssetManager::LoadModel("Assets/Models/Environment/Ground.obj"));
    ground->SetTexture(MiraEngine::TextureSlot::Color, groundTextures + "Color.jpg");
    ground->SetTexture(MiraEngine::TextureSlot::Normal, groundTextures + "NormalGL.jpg");
    ground->SetTexture(MiraEngine::TextureSlot::Occlusion, groundTextures + "AmbientOcclusion.jpg");
    //ground->SetTexture(MiraEngine::TextureSlot::Roughness, groundTextures + "Roughness.jpg");
    scene.AddObject(std::move(ground));

    // Grass: scattered clumps, kept off the spots where gameplay happens
    const std::string grassFolder = "Assets/Models/Environment/Grass/";

    const std::vector<std::string> grassModels = {
        "GrassClumpGreenA", "GrassClumpGreenB",
        "GrassClumpMidA",   "GrassClumpMidB",
        "GrassPatchGreen",  "GrassPatchMid",
    };

    struct ClearZone
    {
        glm::vec3 center;
        float radius;
    };

    const std::vector<ClearZone> clearZones = {
        {{0.9f, 0.0f, -4.0f}, 1.5f},  // player start
        {{2.0f, 0.0f, 0.0f}, 0.8f},   // sword
        {{-2.0f, 0.0f, 3.0f}, 0.8f},  // potion
        {{-1.7f, 0.0f, 1.0f}, 0.8f},  // dungeon key
        {{1.1f, 0.0f, 12.0f}, 1.5f},  // chest
        {{1.5f, 0.0f, 3.0f}, 1.5f},   // door
        {{-3.3f, 0.0f, 2.0f}, 1.0f},  // elder
    };

    std::mt19937 rng(1234); // fixed seed: same layout every run
    std::uniform_real_distribution<float> position(-18.0f, 18.0f);
    std::uniform_real_distribution<float> angle(0.0f, 360.0f);
    std::uniform_real_distribution<float> size(0.8f, 1.4f);
    std::uniform_int_distribution<std::size_t> pick(0, grassModels.size() - 1);

    const int grassCount = 150;
    int placed = 0;

    while (placed < grassCount)
    {
        const glm::vec3 spot(position(rng), 0.0f, position(rng));

        bool blocked = false;
        for (const ClearZone& zone : clearZones)
        {
            if (glm::distance(spot, zone.center) < zone.radius)
            {
                blocked = true;
                break;
            }
        }

        if (blocked)
        {
            continue; // pick another spot
        }

        // Each variant uses the texture matching its name ("Mid" variants use the mid-green texture).
        const std::string variant = grassModels[pick(rng)];
        const std::string texture = variant.find("Mid") != std::string::npos ? "GrassMid.png" : "GrassGreen.png";

        auto grass = std::make_unique<MiraEngine::GameObject>("Grass");
        grass->SetModel(MiraEngine::AssetManager::LoadModel(grassFolder + variant + ".obj"));
        grass->SetTexture(MiraEngine::TextureSlot::Color, grassFolder + texture);
        grass->GetTransform().SetPosition(spot);
        grass->GetTransform().SetRotation(glm::vec3(0.0f, angle(rng), 0.0f));
        grass->GetTransform().SetScale(glm::vec3(size(rng)));
        scene.AddObject(std::move(grass));

        ++placed;
    }
}

MiraGame::Player& InitializePlayer(MiraEngine::Scene& scene)
{
    auto player = std::make_unique<MiraGame::Player>(scene);
    MiraGame::Player& result = *player;
    player->GetTransform().SetPosition(glm::vec3(0.886f, 0.0f, -3.98f));
    scene.AddObject(std::move(player));
    return result;
}

void InitializeEnemy(MiraEngine::Scene& scene, MiraGame::Player& player)
{
    auto enemy = std::make_unique<MiraGame::Enemy>(player, scene, glm::vec3(5.0f, 0.0f, 5.0f));
    enemy->GetMaterial().color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    scene.AddObject(std::move(enemy));
}

void InitializeInteractions(MiraEngine::Scene& scene)
{
    // Sword
    MiraGame::ItemData swordData;
    swordData.name = "Sword";
    swordData.type = MiraGame::ItemType::Weapon;
    swordData.damage = 20;
    auto sword = std::make_unique<MiraGame::Item>(swordData);
    auto swordModel = MiraEngine::AssetManager::LoadModel("Assets/Models/Interactable/Sword/Sword.obj");
    sword->SetModel(swordModel);

    const std::string swordFolder = "Assets/Models/Interactable/Sword/";
    sword->SetTexture(MiraEngine::TextureSlot::Color, swordFolder + "all.001_Base_color.png");
    sword->SetTexture(MiraEngine::TextureSlot::Normal, swordFolder + "all.001_Normal_OpenGL.png");
    sword->SetTexture(MiraEngine::TextureSlot::Occlusion, swordFolder + "all.001_Mixed_AO.png");

    sword->GetTransform().SetRotation(glm::vec3(-90.f, 0.f, 0.f));
    sword->GetTransform().SetScale(glm::vec3(0.1f));
    sword->GetTransform().SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    scene.AddObject(std::move(sword));

    // Potion
    MiraGame::ItemData potionData;
    potionData.name = "Potion";
    potionData.type = MiraGame::ItemType::Consumable;
    potionData.healAmount = 30;
    auto potion = std::make_unique<MiraGame::Item>(potionData);
    potion->GetTransform().SetScale(glm::vec3(0.06f));
    auto potionModel = MiraEngine::AssetManager::LoadModel("Assets/Models/Interactable/Potion/potion.obj");
    potion->SetModel(potionModel);

    const std::string potionFolder = "Assets/Models/Interactable/Potion/";
    potion->SetTexture(MiraEngine::TextureSlot::Color, potionFolder + "potion_lp2_DefaultMaterial_BaseColor.png");
    potion->SetTexture(MiraEngine::TextureSlot::Normal, potionFolder + "potion_lp2_DefaultMaterial_Normal.png");

    potion->GetTransform().SetPosition(glm::vec3(-2.0f, 0.0f, 3.0f));
    scene.AddObject(std::move(potion));

    // Dungeon Key
    MiraGame::ItemData dungeonKeyData;
    dungeonKeyData.name = "Dungeon Key";
    dungeonKeyData.type = MiraGame::ItemType::KeyItem;
    auto dungeonKey = std::make_unique<MiraGame::Item>(dungeonKeyData);
    dungeonKey->GetTransform().SetScale(glm::vec3(0.5f));
    auto dungeonKeyModel = MiraEngine::AssetManager::LoadModel("Assets/Models/Interactable/Key/Key.fbx");
    dungeonKey->SetModel(dungeonKeyModel);

    const std::string keyFolder = "Assets/Models/Interactable/Key/";
    dungeonKey->SetTexture(MiraEngine::TextureSlot::Color, keyFolder + "Key2_albedo.jpeg");
    dungeonKey->SetTexture(MiraEngine::TextureSlot::Normal, keyFolder + "Key2_normal.png");
    dungeonKey->SetTexture(MiraEngine::TextureSlot::Occlusion, keyFolder + "Key2_AO.jpeg"); 
    dungeonKey->GetTransform().SetPosition(glm::vec3(-1.68f, 0.0f, 1.0f));

    scene.AddObject(std::move(dungeonKey));

    // Chest
    MiraGame::ItemData chestPotion;
    chestPotion.name = "Chest Potion";
    chestPotion.type = MiraGame::ItemType::Consumable;
    chestPotion.healAmount = 50;
    auto chest = std::make_unique<MiraGame::Chest>(chestPotion);
    auto chestModel = MiraEngine::AssetManager::LoadModel("Assets/Models/Interactable/Chest/Chest.obj");
    chest->SetModel(chestModel);

    const std::string chestFolder = "Assets/Models/Interactable/Chest/";
    chest->SetTexture(MiraEngine::TextureSlot::Color, chestFolder + "BaseColor.png");

    chest->GetTransform().SetPosition(glm::vec3(1.07f, 0.0f, 12.0f));
    chest->GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    chest->GetTransform().SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
    scene.AddObject(std::move(chest));

    // Door
    auto door = std::make_unique<MiraGame::Door>("Dungeon Key");
    auto doorModel = MiraEngine::AssetManager::LoadModel("Assets/Models/Interactable/Door/Door.fbx");
    door->SetModel(doorModel);
	door->SetTexture(MiraEngine::TextureSlot::Color, "Assets/Models/Interactable/Door/Door.png");
    door->GetTransform().SetPosition(glm::vec3(1.5f, 0.0f, 3.0f));
    door->GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    door->GetTransform().SetScale(glm::vec3(0.005f, 0.005f, 0.005f));
    scene.AddObject(std::move(door));
}

void InitializeNPCs(MiraEngine::Scene& scene)
{
    MiraGame::Dialogue elderDialogue;
    elderDialogue.beforeQuest = { "Greetings, traveler.", "The dungeon lies beyond the locked door.",
                                 "Find the Dungeon Key and you may enter." };
    elderDialogue.questActive = { "Have you found the Dungeon Key yet?", "Search carefully. It must be nearby." };
    elderDialogue.questCompleted = { "Excellent. You found the Dungeon Key.", "The dungeon awaits you." };
    elderDialogue.questTurnedIn = { "Good luck in the dungeon." };

    auto npc = std::make_unique<MiraGame::NPC>("Village Elder", elderDialogue, "Find the Dungeon Key");

    npc->GetMaterial().color = glm::vec4(0, .5, 1, 1);
    npc->GetTransform().SetPosition(glm::vec3(-3.3f, 0.0f, 2.0f));
    npc->GetTransform().SetRotation(glm::vec3(0.f, 120.f, 0.f));
    scene.AddObject(std::move(npc));
}

void InitializeCollisionObjects(MiraEngine::Scene& scene)
{
    auto wallModel = MiraEngine::AssetManager::LoadModel("Assets/Models/Collision/Wall/Wall.obj");
    auto wall1 = std::make_unique<MiraEngine::GameObject>("Wall 1");
    wall1->SetModel(wallModel);
    wall1->GetMaterial().textures.color = MiraEngine::AssetManager::LoadTexture("Assets/Textures/Wall/Wall.jpg");
    wall1->SetCollider(MiraEngine::Collider::Box(glm::vec3(2.95f, 0.7f, 0.15f)));
    wall1->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    wall1->GetTransform().SetRotation(glm::vec3(0.0f, -90.0f, -5.0f));
    scene.AddObject(std::move(wall1));
    auto wall2 = std::make_unique<MiraEngine::GameObject>("Wall 2");
    wall2->SetModel(wallModel);

    /*wall2->SetDiffuseOverride(
        "Assets/Models/Collision/Wall/tex_u1_v1_normal.jpg"
    );*/

    wall2->SetCollider(MiraEngine::Collider::Box(glm::vec3(2.95f, 0.7f, 0.15f)));
    wall2->GetTransform().SetPosition(glm::vec3(-6.0f, 0.0f, 10.0f));
    wall2->GetTransform().SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    scene.AddObject(std::move(wall2));
}

void InitializeCamera(MiraEngine::Scene& scene, MiraEngine::Camera& camera, MiraGame::Player& player)
{
    auto cameraController = std::make_unique<MiraGame::CameraController>(camera, player);
    scene.AddObject(std::move(cameraController));
}