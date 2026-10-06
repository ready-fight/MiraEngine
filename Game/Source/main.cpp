#include "Core/Application.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"
#include "Interaction/Chest.h"
#include "Interaction/Door.h"
#include "Interaction/NPC.h"
#include "Assets/AssetManager.h"

#include <glm/vec3.hpp>
#include <memory>
#include "Graphics/Model.h"

int main()
{
    MiraEngine::Application application;

    auto player =
        std::make_unique<MiraGame::Player>(
            &application.GetScene()
        );

    MiraGame::ItemData keyData;
    keyData.name = "Dungeon Key";
    keyData.type = MiraGame::ItemType::KeyItem;
    player->AddItem(keyData);

    MiraGame::Player* playerPtr =
        player.get();

    application.GetScene().AddObject(
        std::move(player)
    );

    auto enemy =
        std::make_unique<MiraGame::Enemy>(
            playerPtr
        );

    MiraGame::Enemy* enemyPtr = enemy.get();

    enemy->GetMaterial().color = glm::vec3(0.85, 0, 0);

    enemy->GetTransform().SetPosition(
        glm::vec3(5.0f, 0.0f, 5.0f)
    );

    application.GetScene().AddObject(
        std::move(enemy)
    );

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

    swordModel->SetTexture(
        "Assets/Models/Interactable/Sword/all.001_Base_color.png"
    );

    item->SetModel(swordModel);

    // Temporary placeholder size/location.
    item->GetTransform().SetRotation(
        glm::vec3(-90.f, 0.f, 0.f)
    );
    
    item->GetTransform().SetScale(
        glm::vec3(0.1f)
    );

    item->GetTransform().SetPosition(
        glm::vec3(2.0f, 0.0f, 0.0f)
    );

    application.GetScene().AddObject(
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

    potionModel->SetTexture("Assets/Models/Interactable/Potion/potion_lp2_DefaultMaterial_BaseColor.png");

    potion->SetModel(potionModel);

    potion->GetTransform().SetPosition(
        glm::vec3(-2.0f, 0.0f, 3.0f)
    );

    application.GetScene().AddObject(
        std::move(potion)
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

    chestModel->SetTexture("Assets/Models/Interactable/Chest/BaseColor.png");

    chest->SetModel(chestModel);

    chest->GetTransform().SetPosition(
        glm::vec3(-5.0f, 0.0f, 3.0f)
    );

    application.GetScene().AddObject(
        std::move(chest)
    );

    auto door =
        std::make_unique<MiraGame::Door>("Dungeon Key");

    auto doorModel =
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Interactable/Door/Door.fbx"
        );

    doorModel->SetTexture("Assets/Models/Interactable/Door/Door.png");

    door->SetModel(doorModel);

    door->GetTransform().SetPosition(
        glm::vec3(3.0f, 0.0f, 3.0f)
    );

    door->GetTransform().SetRotation(
        glm::vec3(0.0f, 90.0f, 0.0f)
    );

    door->GetTransform().SetScale(
        glm::vec3(0.005f, 0.005f, 0.005f)
    );

    application.GetScene().AddObject(
        std::move(door)
    );

    auto npc =
        std::make_unique<MiraGame::NPC>(
            "Village Elder",
            "The dungeon lies beyond the locked door."
        );

    npc->GetMaterial().color = glm::vec3(0, .5, 1);

    npc->GetTransform().SetPosition(
        glm::vec3(-3.3f, 0.0f, 2.0f)
    );

    npc->GetTransform().SetRotation(
        glm::vec3(0.f, 120.f, 0.f)
    );

    application.GetScene().AddObject(
        std::move(npc)
    );

    application.Run();


    return 0;
}