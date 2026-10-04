#pragma once

#include "Scene/GameObject.h"

namespace MiraGame
{
    class Item : public MiraEngine::GameObject
    {
    public:
        Item();

        void Pickup();

        bool IsPickedUp() const;

    private:
        bool m_pickedUp = false;
    };
}