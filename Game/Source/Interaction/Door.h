#pragma once

#include "Scene/GameObject.h"
#include "Interaction/Interactable.h"

#include <string>

namespace MiraGame
{
    class Door :
        public MiraEngine::GameObject,
        public Interactable
    {
    public:
        explicit Door(
            const std::string& requiredKey = ""
        );

        bool CanInteract() const override;

        void Interact(
            Player& player
        ) override;

        void Update(float deltaTime) override;

    private:
        bool m_isOpen = false;
        float m_targetYaw = 0.0f;
        float m_openSpeed = 90.0f;
        std::string m_requiredKey;
    };
}