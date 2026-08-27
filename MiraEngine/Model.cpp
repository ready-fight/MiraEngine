#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace Wave {

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

	void Model::Load(const std::string& filePath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices | aiProcess_GenSmoothNormals);

		if (scene == nullptr || scene->mRootNode == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
		{
			throw std::runtime_error("Failed to load model: " + std::string(importer.GetErrorString()));
		}

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(mesh));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh)
	{
		std::vector<Vertex> vertices;
		std::vector<std::uint32_t> indices;

		vertices.reserve(mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			const aiVector3D& position = mesh->mVertices[i];
			const aiVector3D& normal = mesh->mNormals[i];

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
					}
				}
			);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
				
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		return std::make_unique<Mesh>(vertices, indices);
	}
}
