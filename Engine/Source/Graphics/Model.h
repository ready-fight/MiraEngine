#pragma once

#include "Mesh.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

struct aiMesh;
struct aiNode;
struct aiScene;

namespace Mira {
	class Model
	{
		public:
			explicit Model(const std::string& filePath);
			void Draw() const;

		private:

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

			struct AnimationClip
			{
				std::string name;
				double duration = 0.0;
				double ticksPerSecond = 0.0;
			};

			void Load(const std::string& filePath);
			void ProcessNode(aiNode* node, const aiScene* scene);
			void ReadHierarchyData(NodeData& destination, const aiNode* source);


			std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh);
			std::vector<std::unique_ptr<Mesh>> m_meshes;

			std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
			int m_boneCounter = 0;
			NodeData m_rootNode;
			std::vector<AnimationClip> m_animations;
	};
}


