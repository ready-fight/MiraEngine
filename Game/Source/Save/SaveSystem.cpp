#include "Save/SaveSystem.h"

#include "Player/Player.h"
#include "Scene/Transform.h"

#include <fstream>
#include <iomanip>
#include <limits>

namespace MiraGame {
    bool SaveSystem::SavePlayer(const Player& player,
                                const std::string& filePath) {
        std::ofstream file(filePath);

        if (!file.is_open()) {
            return false;
        }

        const glm::vec3& position = player.GetTransform().GetPosition();

        file << std::setprecision(std::numeric_limits<float>::max_digits10)
             << position.x << ' ' << position.y << ' ' << position.z << '\n';

        return true;
    }

    bool SaveSystem::LoadPlayer(Player& player, const std::string& filePath) {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            return false;
        }

        glm::vec3 position;

        if (!(file >> position.x >> position.y >> position.z)) {
            return false;
        }

        player.GetTransform().SetPosition(position);

        return true;
    }
}