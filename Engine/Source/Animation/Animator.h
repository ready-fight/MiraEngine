#pragma once

#include "Graphics/Model.h"

#include <glm/mat4x4.hpp>

#include <vector>

namespace MiraEngine
{
    class Animator
    {
    public:
        explicit Animator(const Model& model);

        void PlayAnimation(size_t animationIndex, bool looping = true);
        void StopAnimation();
        void Update(float deltaTime);
        void SetModel(const Model& model);

        const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

    private:

        void CalculateNodeTransforms(
            const Model::NodeData& node,
            const glm::mat4& parentTransform
        );

        const Model* m_model;

        const AnimationClip* m_currentAnimation = nullptr;

        double m_currentTime = 0.0;

        bool m_looping = true;

        std::vector<glm::mat4> m_finalBoneMatrices;
    };
}