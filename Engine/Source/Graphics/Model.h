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

namespace MiraEngine
{

    class Shader;

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
        };

        explicit Model(const std::string& filePath);
        void Draw(const Shader& shader, const TextureSet& textures = {}, const std::vector<glm::mat4>* boneTransforms = nullptr) const;
        void AddAnimations(const std::vector<std::shared_ptr<AnimationClip>>& clips) { m_animations.insert(m_animations.end(), clips.begin(), clips.end()); }
        const NodeData& GetRootNode() const;
        const std::vector<std::shared_ptr<AnimationClip>>& GetAnimations() const;
        const std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap() const;
        int GetBoneCount() const;
        const glm::mat4& GetGlobalInverseTransform() const;

        static const int MaxBones = 100;

    private:
        void Load(const std::string& filePath);
        void ProcessNode(aiNode* node, const aiScene* scene);
        void ReadHierarchyData(NodeData& destination, const aiNode* source);
        std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        NodeData m_rootNode;
        std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
        std::vector<std::unique_ptr<Mesh>> m_meshes;
        std::vector<std::shared_ptr<AnimationClip>> m_animations;
        glm::mat4 m_globalInverseTransform{1.0f};
        int m_boneCounter = 0;
    };
}
