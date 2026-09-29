#include "Enemy.h"

namespace Mira {
    Enemy::Enemy(
        const std::shared_ptr<Model>& characterModel
    )
        : GameObject(
            characterModel,
            Transform(
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, -180.0f, 0.0f),
                glm::vec3(0.05f)
            ),
            glm::vec3(1.0f, 0.0f, 0.0f),
            "Enemy"
        )
    {
    }
}