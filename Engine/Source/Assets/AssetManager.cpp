#include "Assets/AssetManager.h"

#include "Graphics/Model.h"
#include "Animation/Animation.h"
#include "Graphics/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <filesystem>
#include <stdexcept>
#include <utility>

namespace MiraEngine
{
	std::unordered_map<std::string, std::shared_ptr<Model>> AssetManager::s_models;
	std::unordered_map<std::string, std::vector<std::shared_ptr<AnimationClip>>> AssetManager::s_animations;
	std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::s_textures;


	std::shared_ptr<Model> AssetManager::LoadModel(const std::string& path)
	{
		if (!std::filesystem::is_regular_file(path))
		{
			throw std::runtime_error("Model file does not exist: " + path);
		}

		const auto it = s_models.find(path);

		if (it != s_models.end())
		{
			return it->second;
		}

		auto model = std::make_shared<Model>(path);
		s_models[path] = model;

		return model;
	}

	const std::vector<std::shared_ptr<AnimationClip>>& AssetManager::LoadAnimations(const std::string& path)
	{
		if (const auto cached = s_animations.find(path); cached != s_animations.end())
		{
			return cached->second;
		}

		if (!std::filesystem::is_regular_file(path))
		{
			throw std::runtime_error("Animation file does not exist: " + path);
		}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, 0);

		if (!scene)
		{
			throw std::runtime_error("Failed to load animation(s): " + path + "\n" + importer.GetErrorString());
		}

		if (scene->mNumAnimations == 0)
		{
			throw std::runtime_error("File contains no animations: " + path);
		}

		std::vector<std::shared_ptr<AnimationClip>> clips;
		clips.reserve(scene->mNumAnimations);

		for (unsigned int animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex)
		{
			const aiAnimation* animation = scene->mAnimations[animationIndex];
			auto clip = std::make_shared<AnimationClip>();
			clip->name = animation->mName.C_Str();
			clip->duration = animation->mDuration;
			clip->ticksPerSecond = animation->mTicksPerSecond;
			clip->channels.reserve(animation->mNumChannels);

			for (unsigned int channelIndex = 0; channelIndex < animation->mNumChannels; ++channelIndex)
			{
				const aiNodeAnim* channel = animation->mChannels[channelIndex];
				BoneAnimation boneAnimation;
				boneAnimation.boneName = channel->mNodeName.C_Str();

				// Positions
				boneAnimation.positions.reserve(channel->mNumPositionKeys);

				for (unsigned int i = 0; i < channel->mNumPositionKeys; ++i)
				{
					const aiVectorKey& key = channel->mPositionKeys[i];

					boneAnimation.positions.push_back({ glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z), key.mTime });
				}

				// Rotations
				boneAnimation.rotations.reserve(channel->mNumRotationKeys);

				for (unsigned int i = 0; i < channel->mNumRotationKeys; ++i)
				{
					const aiQuatKey& key = channel->mRotationKeys[i];

					boneAnimation.rotations.push_back(
						{ glm::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z), key.mTime });
				}

				// Scales
				boneAnimation.scales.reserve(channel->mNumScalingKeys);

				for (unsigned int i = 0; i < channel->mNumScalingKeys; ++i)
				{
					const aiVectorKey& key = channel->mScalingKeys[i];

					boneAnimation.scales.push_back({ glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z), key.mTime });
				}

				clip->channels.push_back(std::move(boneAnimation));
			}

			clips.push_back(std::move(clip));
		}

		return s_animations.emplace(path, std::move(clips)).first->second;
	}

	std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& path)
	{

		if (!std::filesystem::is_regular_file(path))
		{
			throw std::runtime_error("Texture file does not exist: " + path);
		}

		const auto it = s_textures.find(path);

		if (it != s_textures.end())
		{
			return it->second;
		}

		auto texture = std::make_shared<Texture>(path);
		s_textures[path] = texture;

		return texture;
	}

	void AssetManager::Clear()
	{
		s_models.clear();
		s_animations.clear();
		s_textures.clear();
	}
}