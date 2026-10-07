#include "Interaction/Door.h"
#include "Player/Player.h"
#include "Collision/BoxCollider.h"
#include "Collision/SphereCollider.h"

#include <iostream>

namespace MiraGame
{
    Door::Door(
        const std::string& requiredKey
    )
        : MiraEngine::GameObject("Door"),
        m_requiredKey(requiredKey)
    {
        SetCollider(
            std::make_unique<
            MiraEngine::BoxCollider
            >(
                glm::vec3(
                    0.8f,
                    1.5f,
                    0.2f
                )
            )
        );
    }

    bool Door::CanInteract() const
    {
        return !m_isOpen;
    }

    void Door::Interact(
        Player& player
    )
    {
        if (m_isOpen)
        {
            return;
        }

        if (
            !m_requiredKey.empty() &&
            !player.HasItem(m_requiredKey)
            )
        {
            std::cout
                << "Door locked. Requires: "
                << m_requiredKey
                << "\n";

            return;
        }

        if (!m_requiredKey.empty())
        {
            player.RemoveItem(
                m_requiredKey
            );

            std::cout
                << "Used: "
                << m_requiredKey
                << "\n";
        }

        const float currentYaw =
            GetTransform().GetRotation().y;

        m_targetYaw =
            currentYaw + 90.0f;

        m_isOpen = true;

        std::cout
            << "Door opened\n";
    }

    void Door::Update(float deltaTime)
    {
        if (!m_isOpen)
        {
            return;
        }

        glm::vec3 rotation =
            GetTransform().GetRotation();

        rotation.y +=
            m_openSpeed * deltaTime;

        if (rotation.y > m_targetYaw) {
            rotation.y =
                m_targetYaw;

            SetCollider(nullptr);
        }

        GetTransform().SetRotation(
            rotation
        );
    }
}