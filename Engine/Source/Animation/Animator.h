#pragma once

#include "Graphics/Model.h"

#include <glm/mat4x4.hpp>

#include <vector>

namespace Mira
{
    class Animator
    {
    public:
        explicit Animator(const Model& model);

        void PlayAnimation(size_t animationIndex);
        void Update(float deltaTime);

        const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

    private:

        void CalculateNodeTransforms(
            const Model::NodeData& node,
            const glm::mat4& parentTransform
        );

        const Model& m_model;

        const AnimationClip* m_currentAnimation = nullptr;

        double m_currentTime = 0.0;

        std::vector<glm::mat4> m_finalBoneMatrices;
    };
}