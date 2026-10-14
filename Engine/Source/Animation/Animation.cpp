#include "Animation/Animation.h"

#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace MiraEngine
{
    namespace
    {
        // Blends between the two keyframes either side of `time`. Before the first key or after the
        // last, holds that key's value. A channel with no keys returns `fallback`.
        template <typename T, typename Blend>
        T Sample(const std::vector<Keyframe<T>>& keys, double time, const T& fallback, Blend blend)
        {
            if (keys.empty())
            {
                return fallback;
            }

            if (time <= keys.front().time)
            {
                return keys.front().value;
            }

            for (std::size_t i = 0; i + 1 < keys.size(); ++i)
            {
                const Keyframe<T>& current = keys[i];
                const Keyframe<T>& next = keys[i + 1];

                if (time < next.time)
                {
                    const float t = static_cast<float>((time - current.time) / (next.time - current.time));
                    return blend(current.value, next.value, t);
                }
            }

            return keys.back().value;
        }
    }

    glm::mat4 CalculateBoneTransform(const BoneAnimation& animation, double time)
    {
        const auto lerp = [](const glm::vec3& a, const glm::vec3& b, float t) { return glm::mix(a, b, t); };
        const auto slerp = [](const glm::quat& a, const glm::quat& b, float t)
            { return glm::normalize(glm::slerp(a, b, t)); };

        const glm::vec3 position = Sample(animation.positions, time, glm::vec3(0.0f), lerp);
        const glm::quat rotation = Sample(animation.rotations, time, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), slerp);
        const glm::vec3 scale = Sample(animation.scales, time, glm::vec3(1.0f), lerp);

        return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
    }
}