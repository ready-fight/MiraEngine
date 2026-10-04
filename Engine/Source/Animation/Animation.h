#pragma once

#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace MiraEngine
{
    struct PositionKey
    {
        glm::vec3 position;
        double time;
    };

    struct RotationKey
    {
        glm::quat rotation;
        double time;
    };

    struct ScaleKey
    {
        glm::vec3 scale;
        double time;
    };

    struct BoneAnimation
    {
        std::string boneName;

        std::vector<PositionKey> positions;
        std::vector<RotationKey> rotations;
        std::vector<ScaleKey> scales;
    };

    struct AnimationClip
    {
        std::string name;

        double duration = 0.0;
        double ticksPerSecond = 0.0;

        std::vector<BoneAnimation> channels;
    };

    glm::vec3 InterpolatePosition(
        const BoneAnimation& animation,
        double time
    );

    glm::quat InterpolateRotation(
        const BoneAnimation& animation,
        double time
    );

    glm::vec3 InterpolateScale(
        const BoneAnimation& animation,
        double time
    );

    glm::mat4 CalculateBoneTransform(
        const BoneAnimation& animation,
        double time
    );
}