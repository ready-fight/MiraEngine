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
#include <initializer_list>

namespace
{
    void SetVertexBoneData(MiraEngine::Vertex& vertex, int boneID, float weight)
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

    std::shared_ptr<MiraEngine::Texture> LoadMaterialTexture(const aiMaterial* material,
        std::initializer_list<aiTextureType> types,
        const std::string& directory)
    {
        for (aiTextureType type : types)
        {
            aiString texturePath;

            if (material->GetTexture(type, 0, &texturePath) != AI_SUCCESS)
            {
                continue;
            }

            const std::filesystem::path fullPath =
                std::filesystem::path(directory) / std::filesystem::path(texturePath.C_Str()).filename();

            if (std::filesystem::is_regular_file(fullPath))
            {
                return std::make_shared<MiraEngine::Texture>(fullPath.string());
            }
        }

        return nullptr;
    }
}

namespace MiraEngine
{

    Model::Model(const std::string& filePath)
    {
        Load(filePath);

    }

    void Model::Draw(const Shader& shader, const TextureSet& textures) const
    {
        for (const auto& mesh : m_meshes)
        {
            mesh->Draw(shader, textures);
        }
    }

    void Model::Load(const std::string& filePath)
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                               aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (scene == nullptr || scene->mRootNode == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
        {
            std::cerr << "Assimp error: " << importer.GetErrorString() << '\n';
            throw std::runtime_error("Failed to load model: " + std::string(importer.GetErrorString()));
        }

        ProcessNode(scene->mRootNode, scene);
        if (m_boneCounter > MaxBones)
        {
            throw std::runtime_error(filePath + " has " + std::to_string(m_boneCounter)
                + " bones; the renderer supports at most " + std::to_string(MaxBones));
        }
        ReadHierarchyData(m_rootNode, scene->mRootNode);
        m_globalInverseTransform = glm::inverse(ConvertMatrix(scene->mRootNode->mTransformation));
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

    void Model::ReadHierarchyData(NodeData& destination, const aiNode* source)
    {
        destination.name = source->mName.C_Str();
        destination.transform = ConvertMatrix(source->mTransformation);
        destination.children.clear();
        destination.children.reserve(source->mNumChildren);

        for (unsigned int i = 0; i < source->mNumChildren; ++i)
        {
            NodeData child;
            ReadHierarchyData(child, source->mChildren[i]);
            destination.children.push_back(std::move(child));
        }
    }

    const Model::NodeData& Model::GetRootNode() const
    {
        return m_rootNode;
    }

    const std::vector<std::shared_ptr<AnimationClip>>& Model::GetAnimations() const
    {
        return m_animations;
    }

    void Model::AddAnimation(std::shared_ptr<AnimationClip> clip)
    {
        m_animations.push_back(clip);
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
            Vertex vertex;
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

            if (mesh->HasTextureCoords(0))
            {
                vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }

            if (mesh->HasTangentsAndBitangents())
            {
                const glm::vec3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                const glm::vec3 bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);

                const float handedness = glm::dot(glm::cross(vertex.normal, tangent), bitangent) < 0.0f ? -1.0f : 1.0f;
                vertex.tangent = glm::vec4(tangent, handedness);
            }

            vertices.push_back(vertex);
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

            for (unsigned int weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
            {
                const aiVertexWeight& weight = bone->mWeights[weightIndex];
                const unsigned int vertexID = weight.mVertexId;
                SetVertexBoneData(vertices[vertexID], boneID, weight.mWeight);
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

        return std::make_unique<Mesh>(vertices, indices);
    }

    const glm::mat4& Model::GetGlobalInverseTransform() const
    {
        return m_globalInverseTransform;
    }
}
