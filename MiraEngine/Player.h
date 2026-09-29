#pragma once

#include "Character.h"

namespace Mira
{
    class Player : public Character
    {
        public:
            Player(const std::shared_ptr<Model>& characterModel);
        
        };
}