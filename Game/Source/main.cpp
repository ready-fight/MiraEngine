#include "Core/Application.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"
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
            "Assets/Models/Sword/Sword.obj"
        );

    swordModel->SetTexture(
        "Assets/Models/Sword/all.001_Base_color.png"
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

    application.Run();


    return 0;
}