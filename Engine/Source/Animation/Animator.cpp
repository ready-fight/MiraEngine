#include "Animation/Animator.h"

namespace Mira
{
    Animator::Animator(const Model& model)
        : m_model(model)
    {
        m_finalBoneMatrices.resize(
            model.GetBoneCount(),
            glm::mat4(1.0f)
        );
    }

    void Animator::PlayAnimation(size_t animationIndex)
    {
        const auto& animations = m_model.GetAnimations();

        if (animationIndex >= animations.size())
        {
            return;
        }

        m_currentAnimation = &animations[animationIndex];
        m_currentTime = 0.0;
    }

    void Animator::Update(float deltaTime)
    {
        if (!m_currentAnimation)
        {
            return;
        }

        const double ticksPerSecond =
            m_currentAnimation->ticksPerSecond != 0.0
            ? m_currentAnimation->ticksPerSecond
            : 25.0;

        m_currentTime +=
            static_cast<double>(deltaTime) * ticksPerSecond;

        m_currentTime = std::fmod(
            m_currentTime,
            m_currentAnimation->duration
        );

        CalculateNodeTransforms(
            m_model.GetRootNode(),
            glm::mat4(1.0f)
        );
    }

    const std::vector<glm::mat4>&
        Animator::GetFinalBoneMatrices() const
    {
        return m_finalBoneMatrices;
    }

    void Animator::CalculateNodeTransforms(
        const Model::NodeData& node,
        const glm::mat4& parentTransform
    )
    {
        glm::mat4 localTransform = node.transform;

        for (const BoneAnimation& channel : m_currentAnimation->channels)
        {
            if (channel.boneName == node.name)
            {
                localTransform =
                    Mira::CalculateBoneTransform(
                        channel,
                        m_currentTime
                    );

                break;
            }
        }

        const glm::mat4 globalTransform =
            parentTransform * localTransform;

        const auto& boneMap =
            m_model.GetBoneInfoMap();

        const auto bone = boneMap.find(node.name);

        if (bone != boneMap.end())
        {
            const int boneID = bone->second.id;

            m_finalBoneMatrices[boneID] =
                m_model.GetGlobalInverseTransform()
                * globalTransform
                * bone->second.offset;
        }

        for (const Model::NodeData& child : node.children)
        {
            CalculateNodeTransforms(
                child,
                globalTransform
            );
        }
    }
}