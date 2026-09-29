#include "Character.h"


namespace Mira {
    Character::Character(const std::shared_ptr<Model>& model, const Transform& transform, glm::vec3 color, const char* name)
        : GameObject(model, transform, color, name)
    {
    }
}