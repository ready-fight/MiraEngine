#pragma once

#include <string>

namespace MiraGame {
    class Player;

    class SaveSystem {
      public:
        static bool SavePlayer(const Player& player,
                               const std::string& filePath = "savegame.txt");

        static bool LoadPlayer(Player& player,
                               const std::string& filePath = "savegame.txt");
    };
}