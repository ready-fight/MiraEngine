#pragma once

namespace MiraGame
{
    class Player;

    class Interactable
    {
    public:
        virtual ~Interactable() = default;

        virtual bool CanInteract() const = 0;

        virtual void Interact(
            Player& player
        ) = 0;
    };
}