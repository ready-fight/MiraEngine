#include "Core/Application.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"
#include "Assets/AssetManager.h"

#include <glm/vec3.hpp>
#include <memory>

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

    auto item =
        std::make_unique<MiraGame::Item>();

    item->SetModel(
        MiraEngine::AssetManager::LoadModel(
            "Assets/Models/Sword/espada_low.obj"
        )
    );

    // Temporary placeholder size/location.
    /*item->GetTransform().SetScale(
        glm::vec3(0.3f)
    );*/

    item->GetTransform().SetPosition(
        glm::vec3(2.0f, 0.0f, 0.0f)
    );

    application.GetScene().AddObject(
        std::move(item)
    );

    application.Run();


    return 0;
}