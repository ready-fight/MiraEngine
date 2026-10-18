#include "Animation/Animator.h"
#include <cmath>

namespace MiraEngine
{
    Animator::Animator(const Model& model)
        : m_model(&model)
    {
        m_currentAnimation = nullptr;
        m_currentTime = 0.0;
        m_finalBoneMatrices.assign(model.GetBoneCount(), glm::mat4(1.0f));
        CalculateNodeTransforms(
            m_model->GetRootNode(), glm::mat4(1.0f));
    }

    void Animator::PlayAnimation(const std::string& animationName, bool looping)
    {
        const auto& animations = m_model->GetAnimations();

        for (const auto& animation : animations)
        {
            if (animation->name == animationName)
            {
                m_currentAnimation = animation.get();
                m_currentTime = 0.0;
                m_looping = looping;
                return;
            }
        }
    }

    void Animator::StopAnimation()
    {
        m_currentAnimation = nullptr;
        m_currentTime = 0.0;

        m_finalBoneMatrices.assign(
            m_model->GetBoneCount(), glm::mat4(1.0f));

        CalculateNodeTransforms(
            m_model->GetRootNode(), glm::mat4(1.0f));
    }

    void Animator::Update(float deltaTime)
    {
        if (!m_currentAnimation)
        {
            return;
        }

        const double ticksPerSecond = m_currentAnimation->ticksPerSecond != 0.0 ? m_currentAnimation->ticksPerSecond
                                                                                : 25.0;

        m_currentTime += static_cast<double>(deltaTime) * ticksPerSecond;

        if (m_looping)
        {
            m_currentTime = std::fmod(m_currentTime, m_currentAnimation->duration);
        }
        else if (m_currentTime > m_currentAnimation->duration)
        {
            m_currentTime = m_currentAnimation->duration;
        }

        CalculateNodeTransforms(m_model->GetRootNode(), glm::mat4(1.0f));
    }

    void Animator::SetModel(const Model& model)
    {
        m_model = &model;
        StopAnimation();
    }

    const std::vector<glm::mat4>& Animator::GetFinalBoneMatrices() const
    {
        return m_finalBoneMatrices;
    }

    void Animator::CalculateNodeTransforms(const Model::NodeData& node, const glm::mat4& parentTransform)
    {
        glm::mat4 localTransform = node.transform;

        if (m_currentAnimation) {
            for (const BoneAnimation& channel : m_currentAnimation->channels)
            {
                if (channel.boneName == node.name)
                {
                    localTransform = CalculateBoneTransform(channel, m_currentTime);

                    break;
                }
            }
        }

        const glm::mat4 globalTransform = parentTransform * localTransform;
        const auto& boneMap = m_model->GetBoneInfoMap();
        const auto bone = boneMap.find(node.name);

        if (bone != boneMap.end())
        {
            const int boneID = bone->second.id;
            m_finalBoneMatrices[boneID] = m_model->GetGlobalInverseTransform() * globalTransform;
        }

        for (const Model::NodeData& child : node.children)
        {
            CalculateNodeTransforms(child, globalTransform);
        }
    }
}