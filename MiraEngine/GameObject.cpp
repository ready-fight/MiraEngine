#include "GameObject.h"


namespace Mira {
	void GameObject::Move(const glm::vec3& direction, float deltaTime)
	{
        if (glm::length(direction) == 0.0f)
        {
            return;
        }

        const glm::vec3 normalizedDirection =
            glm::normalize(direction);

        GetTransform().Translate(
            normalizedDirection *
            m_movementSpeed *
            deltaTime
        );
	}
}

