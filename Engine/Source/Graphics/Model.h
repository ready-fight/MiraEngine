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
#include <limits>

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
        void Draw(const Shader& shader, const glm::mat4& modelMatrix, const TextureSet& textures = {}, const std::vector<glm::mat4>* boneTransforms = nullptr) const;
        void AddAnimations(const std::vector<std::shared_ptr<AnimationClip>>& clips) { m_animations.insert(m_animations.end(), clips.begin(), clips.end()); }
        const NodeData& GetRootNode() const;
        const std::vector<std::shared_ptr<AnimationClip>>& GetAnimations() const;
        const std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap() const;
        int GetBoneCount() const;
        const glm::mat4& GetGlobalInverseTransform() const;
        const glm::vec3& GetBoundsMin() const { return m_boundsMin; }
        const glm::vec3& GetBoundsMax() const { return m_boundsMax; }

        static const int MaxBones = 100;

    private:
        void Load(const std::string& filePath);
        void ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
        void ReadHierarchyData(NodeData& destination, const aiNode* source);
        void ExpandBounds(const aiMesh* mesh, const glm::mat4& transform);

        struct MeshEntry
        {
            std::unique_ptr<Mesh> mesh;
            glm::mat4 transform{ 1.0f };
        };

        std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        NodeData m_rootNode;
        std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
        std::vector<MeshEntry> m_meshes;
        std::vector<std::shared_ptr<AnimationClip>> m_animations;
        glm::mat4 m_globalInverseTransform{1.0f};
        int m_boneCounter = 0;
        glm::vec3 m_boundsMin{ std::numeric_limits<float>::max() };
        glm::vec3 m_boundsMax{ std::numeric_limits<float>::lowest() };
    };
}
