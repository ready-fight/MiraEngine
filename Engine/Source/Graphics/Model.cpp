#include "Model.h"
#include "Graphics/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

namespace
{
	void SetVertexBoneData(
		MiraEngine::Vertex& vertex,
		int boneID,
		float weight
	)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (vertex.weights[i] == 0.0f)
			{
				vertex.boneIDs[i] = boneID;
				vertex.weights[i] = weight;
				return;
			}
		}
	}

	glm::mat4 ConvertMatrix(const aiMatrix4x4& matrix)
	{
		glm::mat4 result;

		result[0][0] = matrix.a1;
		result[1][0] = matrix.a2;
		result[2][0] = matrix.a3;
		result[3][0] = matrix.a4;

		result[0][1] = matrix.b1;
		result[1][1] = matrix.b2;
		result[2][1] = matrix.b3;
		result[3][1] = matrix.b4;

		result[0][2] = matrix.c1;
		result[1][2] = matrix.c2;
		result[2][2] = matrix.c3;
		result[3][2] = matrix.c4;

		result[0][3] = matrix.d1;
		result[1][3] = matrix.d2;
		result[2][3] = matrix.d3;
		result[3][3] = matrix.d4;

		return result;
	}
}

namespace MiraEngine {

	Model::Model(const std::string& filePath)
	{
		Load(filePath);

	}

	void Model::Draw() const
	{
		for (const auto& mesh : m_meshes)
		{
			mesh->Draw();
		}
	}

	void Model::LoadAnimation(const std::string& filePath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(
			filePath,
			aiProcess_Triangulate
		);

		if (!scene || scene->mNumAnimations == 0)
		{
			throw std::runtime_error(
				"Failed to load animation: " + filePath
			);
		}

		for (unsigned int animationIndex = 0;
			animationIndex < scene->mNumAnimations;
			++animationIndex)
		{
			const aiAnimation* animation =
				scene->mAnimations[animationIndex];

			AnimationClip clip;

			clip.name = animation->mName.C_Str();
			clip.duration = animation->mDuration;
			clip.ticksPerSecond = animation->mTicksPerSecond;

			clip.channels.reserve(animation->mNumChannels);

			for (unsigned int channelIndex = 0;
				channelIndex < animation->mNumChannels;
				++channelIndex)
			{
				const aiNodeAnim* channel =
					animation->mChannels[channelIndex];

				BoneAnimation boneAnimation;

				boneAnimation.boneName =
					channel->mNodeName.C_Str();

				for (unsigned int i = 0;
					i < channel->mNumPositionKeys;
					++i)
				{
					const aiVectorKey& key =
						channel->mPositionKeys[i];

					boneAnimation.positions.push_back({
						glm::vec3(
							key.mValue.x,
							key.mValue.y,
							key.mValue.z
						),
						key.mTime
						});
				}

				for (unsigned int i = 0;
					i < channel->mNumRotationKeys;
					++i)
				{
					const aiQuatKey& key =
						channel->mRotationKeys[i];

					boneAnimation.rotations.push_back({
						glm::quat(
							key.mValue.w,
							key.mValue.x,
							key.mValue.y,
							key.mValue.z
						),
						key.mTime
						});
				}

				for (unsigned int i = 0;
					i < channel->mNumScalingKeys;
					++i)
				{
					const aiVectorKey& key =
						channel->mScalingKeys[i];

					boneAnimation.scales.push_back({
						glm::vec3(
							key.mValue.x,
							key.mValue.y,
							key.mValue.z
						),
						key.mTime
						});
				}

				clip.channels.push_back(
					std::move(boneAnimation)
				);
			}

			m_animations.push_back(std::move(clip));
		}
	}

	void Model::Load(const std::string& filePath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

		if (scene == nullptr || scene->mRootNode == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
		{
			std::cerr << "Assimp error: "
				<< importer.GetErrorString()
				<< '\n';

			throw std::runtime_error("Failed to load model: " + std::string(importer.GetErrorString()));
		}

		m_directory =
			std::filesystem::path(filePath)
			.parent_path()
			.string();

		ProcessNode(scene->mRootNode, scene);

		ReadHierarchyData(
			m_rootNode,
			scene->mRootNode
		);

		m_globalInverseTransform =
			glm::inverse(ConvertMatrix(scene->mRootNode->mTransformation));

		m_animations.clear();
		m_animations.reserve(scene->mNumAnimations);

		for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
		{
			const aiAnimation* animation = scene->mAnimations[i];

			AnimationClip clip;

			clip.name = animation->mName.C_Str();
			clip.duration = animation->mDuration;
			clip.ticksPerSecond = animation->mTicksPerSecond;

			clip.channels.reserve(animation->mNumChannels);

			for (unsigned int channelIndex = 0;
				channelIndex < animation->mNumChannels;
				++channelIndex)
			{
				const aiNodeAnim* channel =
					animation->mChannels[channelIndex];

				BoneAnimation boneAnimation;
				boneAnimation.boneName = channel->mNodeName.C_Str();

				boneAnimation.positions.reserve(channel->mNumPositionKeys);

				for (unsigned int i = 0; i < channel->mNumPositionKeys; ++i)
				{
					const aiVectorKey& key = channel->mPositionKeys[i];

					boneAnimation.positions.push_back({
						glm::vec3(
							key.mValue.x,
							key.mValue.y,
							key.mValue.z
						),
						key.mTime
						});
				}

				boneAnimation.rotations.reserve(channel->mNumRotationKeys);

				for (unsigned int i = 0; i < channel->mNumRotationKeys; ++i)
				{
					const aiQuatKey& key = channel->mRotationKeys[i];

					boneAnimation.rotations.push_back({
						glm::quat(
							key.mValue.w,
							key.mValue.x,
							key.mValue.y,
							key.mValue.z
						),
						key.mTime
						});
				}

				boneAnimation.scales.reserve(channel->mNumScalingKeys);

				for (unsigned int i = 0; i < channel->mNumScalingKeys; ++i)
				{
					const aiVectorKey& key = channel->mScalingKeys[i];

					boneAnimation.scales.push_back({
						glm::vec3(
							key.mValue.x,
							key.mValue.y,
							key.mValue.z
						),
						key.mTime
						});
				}

				clip.channels.push_back(std::move(boneAnimation));
			}

			m_animations.push_back(std::move(clip));
		}
	}

	std::shared_ptr<Texture>
		Model::LoadMaterialTexture(
			aiMaterial* material
		)
	{
		if (
			material->GetTextureCount(
				aiTextureType_DIFFUSE
			) == 0
			)
		{
			return nullptr;
		}

		aiString texturePath;

		if (
			material->GetTexture(
				aiTextureType_DIFFUSE,
				0,
				&texturePath
			) != AI_SUCCESS
			)
		{
			return nullptr;
		}

		const std::filesystem::path fullPath =
			std::filesystem::path(m_directory) /
			texturePath.C_Str();

		return std::make_shared<Texture>(
			fullPath.string()
		);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	void Model::ReadHierarchyData(
		NodeData& destination,
		const aiNode* source
	)
	{
		destination.name = source->mName.C_Str();
		destination.transform = ConvertMatrix(source->mTransformation);

		destination.children.clear();
		destination.children.reserve(source->mNumChildren);

		for (unsigned int i = 0; i < source->mNumChildren; ++i)
		{
			NodeData child;

			ReadHierarchyData(
				child,
				source->mChildren[i]
			);

			destination.children.push_back(std::move(child));
		}
	}

	const Model::NodeData& Model::GetRootNode() const
	{
		return m_rootNode;
	}

	const std::vector<AnimationClip>& Model::GetAnimations() const
	{
		return m_animations;
	}

	const std::unordered_map<std::string, Model::BoneInfo>& Model::GetBoneInfoMap() const
	{
		return m_boneInfoMap;
	}

	int Model::GetBoneCount() const
	{
		return m_boneCounter;
	}

	std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<std::uint32_t> indices;

		vertices.reserve(mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			const aiVector3D& position = mesh->mVertices[i];
			const aiVector3D& normal = mesh->mNormals[i];

			glm::vec2 texCoord(0.0f);

			if (mesh->HasTextureCoords(0))
			{
				texCoord.x = mesh->mTextureCoords[0][i].x;
				texCoord.y = mesh->mTextureCoords[0][i].y;
			}

			vertices.push_back(
				{
					{
						position.x,
						position.y,
						position.z
					},
					{
						normal.x,
						normal.y,
						normal.z
					},
					texCoord
				}
			);
		}

		for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			aiBone* bone = mesh->mBones[boneIndex];

			const std::string boneName = bone->mName.C_Str();

			int boneID = 0;

			auto it = m_boneInfoMap.find(boneName);

			if (it == m_boneInfoMap.end())
			{
				BoneInfo info;

				info.id = m_boneCounter;

				info.offset = ConvertMatrix(bone->mOffsetMatrix);

				m_boneInfoMap[boneName] = info;

				boneID = m_boneCounter;
				++m_boneCounter;
			}
			else
			{
				boneID = it->second.id;
			}

			for (unsigned int weightIndex = 0;
				weightIndex < bone->mNumWeights;
				++weightIndex)
			{
				const aiVertexWeight& weight =
					bone->mWeights[weightIndex];

				const unsigned int vertexID =
					weight.mVertexId;

				SetVertexBoneData(
					vertices[vertexID],
					boneID,
					weight.mWeight
				);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
				
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		std::shared_ptr<Texture> texture;

		if (mesh->mMaterialIndex < scene->mNumMaterials)
		{
			aiMaterial* material =
				scene->mMaterials[
					mesh->mMaterialIndex
				];

			texture =
				LoadMaterialTexture(material);
		}

		return std::make_unique<Mesh>(
			vertices,
			indices,
			texture
		);
	}

	const glm::mat4& Model::GetGlobalInverseTransform() const
	{
		return m_globalInverseTransform;
	}
}
