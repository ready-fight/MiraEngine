#pragma once

#include "Mesh.h"
#include "Animation/Animation.h"
#include "Graphics/Texture.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


struct aiMesh;
struct aiNode;
struct aiScene;
class aiMaterial;

namespace MiraEngine {


	class Model
	{
		public:
			struct NodeData
			{
				std::string name;
				glm::mat4 transform;

				std::vector<NodeData> children;
			};

			struct BoneInfo
			{
				int id;
				glm::mat4 offset;
			};

			explicit Model(const std::string& filePath);
			void Draw() const;
			void LoadAnimation(const std::string& filePath);

			const NodeData& GetRootNode() const;
			const std::vector<AnimationClip>& GetAnimations() const;
			const std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap() const;
			int GetBoneCount() const;
			const glm::mat4& GetGlobalInverseTransform() const;

		private:
			void Load(const std::string& filePath);
			std::shared_ptr<Texture> LoadMaterialTexture(
				aiMaterial* material
			);
			void ProcessNode(aiNode* node, const aiScene* scene);
			void ReadHierarchyData(NodeData& destination, const aiNode* source);

			std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

			NodeData m_rootNode;

			std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
			std::vector<std::unique_ptr<Mesh>> m_meshes;
			std::vector<AnimationClip> m_animations;
			glm::mat4 m_globalInverseTransform{ 1.0f };
			std::string m_directory;

			int m_boneCounter = 0;
	};
}


