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
#include "Interaction/SceneExit.h"
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



void InitializeEnvironment(MiraEngine::Scene& scene)
{
    // Ground: a 40x40 plane with a tiling PBR material (ambientCG Ground037, CC0)
    const std::string grassTextures = "Assets/Textures/Grass/Grass001_1K-JPG_";

    auto ground = std::make_unique<MiraEngine::GameObject>("Ground");
    ground->SetModel("Assets/Models/Environment/Ground.obj");
    ground->SetTexture(MiraEngine::TextureSlot::Color, grassTextures + "Color.jpg");
    ground->SetTexture(MiraEngine::TextureSlot::Normal, grassTextures + "NormalGL.jpg");
    ground->SetTexture(MiraEngine::TextureSlot::Occlusion, grassTextures + "AmbientOcclusion.jpg");
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
        grass->SetModel(grassFolder + variant + ".obj");
        grass->SetTexture(MiraEngine::TextureSlot::Color, grassFolder + texture);
        grass->GetTransform().SetPosition(spot);
        grass->GetTransform().SetRotation(glm::vec3(0.0f, angle(rng), 0.0f));
        grass->GetTransform().SetScale(glm::vec3(size(rng)));
        scene.AddObject(std::move(grass));

        ++placed;
    }
}

MiraGame::Player& InitializePlayer(MiraEngine::Scene& scene, const glm::vec3& position)
{
    auto player = std::make_unique<MiraGame::Player>(scene);
    MiraGame::Player& result = *player;
    player->GetTransform().SetPosition(position);
    scene.AddObject(std::move(player));
    return result;
}

void InitializeEnemy(MiraEngine::Scene& scene, MiraGame::Player& player)
{
    auto enemy = std::make_unique<MiraGame::Enemy>(player, scene, glm::vec3(5.0f, 0.0f, 5.0f));
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
    auto swordModel = ("Assets/Models/Interactable/Sword/Sword.obj");
    sword->SetModel(swordModel);

    const std::string swordFolder = "Assets/Models/Interactable/Sword/";
    sword->SetTexture(MiraEngine::TextureSlot::Color, swordFolder + "all.001_Base_color.png");
    sword->SetTexture(MiraEngine::TextureSlot::Normal, swordFolder + "all.001_Normal_OpenGL.png");
    sword->SetTexture(MiraEngine::TextureSlot::Occlusion, swordFolder + "all.001_Mixed_AO.png");

    sword->GetTransform().SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    sword->GetTransform().SetScale(glm::vec3(0.1f));
    sword->GetTransform().SetPosition(glm::vec3(2.0f, 0.5f, 0.0f));
    scene.AddObject(std::move(sword));

    // Potion
    MiraGame::ItemData potionData;
    potionData.name = "Potion";
    potionData.type = MiraGame::ItemType::Consumable;
    potionData.healAmount = 30;
    auto potion = std::make_unique<MiraGame::Item>(potionData);
    potion->GetTransform().SetScale(glm::vec3(0.06f));
    potion->SetModel("Assets/Models/Interactable/Potion/potion.obj");

    const std::string potionFolder = "Assets/Models/Interactable/Potion/";
    potion->SetTexture(MiraEngine::TextureSlot::Color, potionFolder + "potion_lp2_DefaultMaterial_BaseColor.png");
    potion->SetTexture(MiraEngine::TextureSlot::Normal, potionFolder + "potion_lp2_DefaultMaterial_Normal.png");

    potion->GetTransform().SetPosition(glm::vec3(-2.0f, 0.5f, 3.0f));
    scene.AddObject(std::move(potion));

    // Dungeon Key
    MiraGame::ItemData dungeonKeyData;
    dungeonKeyData.name = "Dungeon Key";
    dungeonKeyData.type = MiraGame::ItemType::KeyItem;
    auto dungeonKey = std::make_unique<MiraGame::Item>(dungeonKeyData);
    dungeonKey->GetTransform().SetScale(glm::vec3(0.005f));
    dungeonKey->SetModel("Assets/Models/Interactable/Key/Key.fbx");

    const std::string keyFolder = "Assets/Models/Interactable/Key/";
    dungeonKey->SetTexture(MiraEngine::TextureSlot::Color, keyFolder + "Key2_albedo.jpeg");
    dungeonKey->SetTexture(MiraEngine::TextureSlot::Normal, keyFolder + "Key2_normal.png");
    dungeonKey->SetTexture(MiraEngine::TextureSlot::Occlusion, keyFolder + "Key2_AO.jpeg");
    dungeonKey->GetTransform().SetPosition(glm::vec3(-1.68f, 0.5f, 1.0f));

    scene.AddObject(std::move(dungeonKey));

    // Chest
    MiraGame::ItemData chestPotion;
    chestPotion.name = "Chest Potion";
    chestPotion.type = MiraGame::ItemType::Consumable;
    chestPotion.healAmount = 50;
    auto chest = std::make_unique<MiraGame::Chest>(chestPotion);
    chest->SetModel("Assets/Models/Interactable/Chest/Chest.obj");

    const std::string chestFolder = "Assets/Models/Interactable/Chest/";
    chest->SetTexture(MiraEngine::TextureSlot::Color, chestFolder + "BaseColor.png");

    chest->GetTransform().SetPosition(glm::vec3(1.07f, 0.5f, 12.0f));
    chest->GetTransform().SetRotation(glm::vec3(0.0f, 90.0f, 0.0f));
    chest->GetTransform().SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
    scene.AddObject(std::move(chest));
}

MiraGame::Door& InitializeDungeonDoor(MiraEngine::Scene& scene)
{
    const std::string doorFolder = "Assets/Models/Interactable/Door/";
    const glm::vec3 position(2.0f, 0.0f, 4.0f);
    const glm::vec3 rotation(0.0f, 0.0f, 0.0f);
    const glm::vec3 scale(0.002f);

    auto frame = std::make_unique<MiraEngine::GameObject>("DoorFrame");
    frame->SetModel(doorFolder + "DoorFrame.fbx");
    frame->SetTexture(MiraEngine::TextureSlot::Color, doorFolder + "door001_BaseColor.png");
    frame->SetTexture(MiraEngine::TextureSlot::Normal, doorFolder + "door001_Normal.png");
    frame->GetTransform().SetPosition(position);
    frame->GetTransform().SetRotation(rotation);
    frame->GetTransform().SetScale(scale);
    scene.AddObject(std::move(frame));

    auto door = std::make_unique<MiraGame::Door>("Dungeon Key");
    MiraGame::Door& result = *door;
    door->SetModel(doorFolder + "DoorLeaf.fbx");
    door->SetTexture(MiraEngine::TextureSlot::Color, doorFolder + "door001_BaseColor.png");
    door->SetTexture(MiraEngine::TextureSlot::Normal, doorFolder + "door001_Normal.png");
    door->GetTransform().SetPosition(position);
    door->GetTransform().SetRotation(rotation);
    door->GetTransform().SetScale(scale);
    door->FitBoxCollider();

    scene.AddObject(std::move(door));
    return result;
}

void InitializeDungeonEntrance(MiraEngine::Scene& scene, MiraGame::Player& player, const MiraGame::Door& door)
{
    auto entrance = std::make_unique<MiraGame::SceneExit>(scene, player, "Dungeon", glm::vec3(0.8f, 1.0f, 0.3f));
    glm::vec3 entrancePosition = door.GetTransform().GetPosition();
    entrancePosition.x += 0.85f;
    entrancePosition.z += 0.5f;
    entrance->GetTransform().SetPosition(entrancePosition);
    entrance->GetTransform().SetRotation(door.GetTransform().GetRotation());
    entrance->SetCondition([&door] { return door.IsOpen(); });
    scene.AddObject(std::move(entrance));
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

    npc->GetTransform().SetPosition(glm::vec3(-3.3f, 0.0f, 2.0f));
    scene.AddObject(std::move(npc));
}

void InitializeCollisionObjects(MiraEngine::Scene& scene)
{
    auto wall = std::make_unique<MiraEngine::GameObject>("Wall 1");
    wall->SetModel("Assets/Models/Collision/Wall/Wall.obj");
    wall->SetTexture(MiraEngine::TextureSlot::Color, "Assets/Models/Collision/Wall/tex_u1_v1_diffuse.jpg");
    wall->SetTexture(MiraEngine::TextureSlot::Normal, "Assets/Models/Collision/Wall/tex_u1_v1_normal.jpg");
    wall->SetCollider(MiraEngine::Collider::Box(glm::vec3(2.95f, 0.7f, 0.15f)));
    wall->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    wall->GetTransform().SetRotation(glm::vec3(0.0f, -90.0f, -5.0f));
    scene.AddObject(std::move(wall));
}

void InitializeCamera(MiraEngine::Scene& scene, MiraEngine::Camera& camera, MiraGame::Player& player)
{
    auto cameraController = std::make_unique<MiraGame::CameraController>(camera, player);
    scene.AddObject(std::move(cameraController));
}

void BuildOverworld(MiraEngine::Scene& scene, MiraEngine::Camera& camera)
{
    InitializeEnvironment(scene);
    MiraGame::Player& player = InitializePlayer(scene, glm::vec3(0.886f, 0.0f, -3.98f));
	assert(&player != nullptr, "Player initialization failed");
    InitializeEnemy(scene, player);
    InitializeInteractions(scene);
    const MiraGame::Door& dungeonDoor = InitializeDungeonDoor(scene);
    InitializeDungeonEntrance(scene, player, dungeonDoor);
    InitializeNPCs(scene);
    //InitializeCollisionObjects(scene);
    InitializeCamera(scene, camera, player);
}

void BuildDungeon(MiraEngine::Scene& scene, MiraEngine::Camera& camera)
{
    // Placeholder: an untextured, dark floor so it's obviously a different place.
    auto floor = std::make_unique<MiraEngine::GameObject>("Floor");
    floor->SetModel("Assets/Models/Environment/Ground.obj");
    floor->GetMaterial().color = glm::vec4(0.35f, 0.3f, 0.28f, 1.0f);
    scene.AddObject(std::move(floor));

    MiraGame::Player& player = InitializePlayer(scene, glm::vec3(0.0f, 0.0f, 0.0f));

    // The way back out: a door model as a marker, with the exit trigger in front of it.
    const glm::vec3 exitPosition(0.0f, 0.0f, -5.0f);
    const std::string doorFolder = "Assets/Models/Interactable/Door/";

    // The door models pivot on the hinge, so shift them left to centre the doorway on the exit.
    const glm::vec3 doorPosition = exitPosition - glm::vec3(2.2f, 0.0f, 0.0f);

    auto exitFrame = std::make_unique<MiraEngine::GameObject>("ExitDoorFrame");
    exitFrame->SetModel(doorFolder + "DoorFrame.fbx");
    exitFrame->SetTexture(MiraEngine::TextureSlot::Color, doorFolder + "door001_BaseColor.png");
    exitFrame->SetTexture(MiraEngine::TextureSlot::Normal, doorFolder + "door001_Normal.png");
    exitFrame->GetTransform().SetPosition(doorPosition);
    exitFrame->GetTransform().SetScale(glm::vec3(0.005f));
    scene.AddObject(std::move(exitFrame));

    auto exitDoor = std::make_unique<MiraEngine::GameObject>("ExitDoor");
    exitDoor->SetModel(doorFolder + "DoorLeaf.fbx");
    exitDoor->SetTexture(MiraEngine::TextureSlot::Color, doorFolder + "door001_BaseColor.png");
    exitDoor->SetTexture(MiraEngine::TextureSlot::Normal, doorFolder + "door001_Normal.png");
    exitDoor->GetTransform().SetPosition(doorPosition);
    exitDoor->GetTransform().SetScale(glm::vec3(0.005f));
    scene.AddObject(std::move(exitDoor));

    InitializeCamera(scene, camera, player);
}