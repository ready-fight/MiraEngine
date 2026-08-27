#pragma once

#include "Mesh.h"

#include <memory>
#include <string>
#include <vector>

struct aiMesh;
struct aiNode;
struct aiScene;

namespace Wave {
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
	};
}


