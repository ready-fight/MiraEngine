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
			void Load(const std::string& filePath);

			void ProcessNode(aiNode* node, const aiScene* scene);

			std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh);

			std::vector<std::unique_ptr<Mesh>> m_meshes;

			struct BoneInfo
			{
				int id;
				glm::mat4 offset;
			};

			std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
			int m_boneCounter = 0;
	};
}


