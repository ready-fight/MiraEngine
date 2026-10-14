#pragma once

#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace MiraEngine
{
    template <typename T>
struct Keyframe
{
    T value;
    double time;
};

struct BoneAnimation
{
    std::string boneName;
    std::vector<Keyframe<glm::vec3>> positions;
    std::vector<Keyframe<glm::quat>> rotations;
    std::vector<Keyframe<glm::vec3>> scales;
};

    struct AnimationClip
    {
        std::string name;
        double duration = 0.0;
        double ticksPerSecond = 0.0;
        std::vector<BoneAnimation> channels;
    };

    glm::vec3 InterpolatePosition(const BoneAnimation& animation, double time);
    glm::quat InterpolateRotation(const BoneAnimation& animation, double time);
    glm::vec3 InterpolateScale(const BoneAnimation& animation, double time);
    glm::mat4 CalculateBoneTransform(const BoneAnimation& animation, double time);
}