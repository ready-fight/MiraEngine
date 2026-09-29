#include "Player.h"

namespace Mira
{
    Player::Player(
        const std::shared_ptr<Model>& characterModel
    )
        : Character(
            characterModel,
            Transform(
                glm::vec3(0.0f, 0.0f, -2.0f),
                glm::vec3(0.0f),
                glm::vec3(0.05f)
            ),
            glm::vec3(1.0f),
            "Player"
        )
    {
    }
}