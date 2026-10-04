#include "Animation/Animation.h"

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace MiraEngine
{
    glm::vec3 InterpolatePosition(
        const BoneAnimation& animation,
        double time
    )
    {
        if (animation.positions.size() == 1)
        {
            return animation.positions[0].position;
        }

        for (size_t i = 0; i < animation.positions.size() - 1; ++i)
        {
            const PositionKey& current = animation.positions[i];
            const PositionKey& next = animation.positions[i + 1];

            if (time < next.time)
            {
                const double length = next.time - current.time;

                const float factor = static_cast<float>(
                    (time - current.time) / length
                    );

                return glm::mix(
                    current.position,
                    next.position,
                    factor
                );
            }
        }

        return animation.positions.back().position;
    }

    glm::quat InterpolateRotation(
        const BoneAnimation& animation,
        double time
    )
    {
        if (animation.rotations.size() == 1)
        {
            return animation.rotations[0].rotation;
        }

        for (size_t i = 0; i < animation.rotations.size() - 1; ++i)
        {
            const RotationKey& current = animation.rotations[i];
            const RotationKey& next = animation.rotations[i + 1];

            if (time < next.time)
            {
                const double length = next.time - current.time;

                const float factor = static_cast<float>(
                    (time - current.time) / length
                    );

                return glm::normalize(
                    glm::slerp(
                        current.rotation,
                        next.rotation,
                        factor
                    )
                );
            }
        }

        return animation.rotations.back().rotation;
    }

    glm::vec3 InterpolateScale(
        const BoneAnimation& animation,
        double time
    )
    {
        if (animation.scales.size() == 1)
        {
            return animation.scales[0].scale;
        }

        for (size_t i = 0; i < animation.scales.size() - 1; ++i)
        {
            const ScaleKey& current = animation.scales[i];
            const ScaleKey& next = animation.scales[i + 1];

            if (time < next.time)
            {
                const double length = next.time - current.time;

                const float factor = static_cast<float>(
                    (time - current.time) / length
                    );

                return glm::mix(
                    current.scale,
                    next.scale,
                    factor
                );
            }
        }

        return animation.scales.back().scale;
    }

    glm::mat4 CalculateBoneTransform(
        const BoneAnimation& animation,
        double time
    )
    {
        const glm::vec3 position =
            InterpolatePosition(animation, time);

        const glm::quat rotation =
            InterpolateRotation(animation, time);

        const glm::vec3 scale =
            InterpolateScale(animation, time);

        const glm::mat4 translationMatrix =
            glm::translate(glm::mat4(1.0f), position);

        const glm::mat4 rotationMatrix =
            glm::mat4_cast(rotation);

        const glm::mat4 scaleMatrix =
            glm::scale(glm::mat4(1.0f), scale);

        return translationMatrix
            * rotationMatrix
            * scaleMatrix;
    }
}