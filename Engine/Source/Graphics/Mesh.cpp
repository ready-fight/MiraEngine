#include "Mesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <GL/glew.h>

#include <cstddef>
#include <utility>

namespace MiraEngine
{
	namespace
	{
		void BindSlot(const Shader& shader, const std::shared_ptr<Texture>& texture, int unit, const char* sampler,
			const char* flag)
		{
			shader.SetInt(flag, texture ? 1 : 0);

			if (texture)
			{
				texture->Bind(unit);
				shader.SetInt(sampler, unit);
			}
		}
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices, std::vector<BoneOffset> boneOffsets) :
		m_indexCount(static_cast<int>(indices.size())), m_boneOffsets(boneOffsets)
	{
		glGenVertexArrays(1, &m_vertexArray);
		glGenBuffers(1, &m_vertexBuffer);
		glGenBuffers(1, &m_indexBuffer);
		glBindVertexArray(m_vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);

		// Vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, texCoord)));
		glEnableVertexAttribArray(2);

		glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, boneIDs)));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, weights)));
		glEnableVertexAttribArray(4);

		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, tangent)));
		glEnableVertexAttribArray(5);
		// End of vertex attributes

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &m_indexBuffer);
		glDeleteBuffers(1, &m_vertexBuffer);
		glDeleteVertexArrays(1, &m_vertexArray);
	}

	void Mesh::Draw(const Shader& shader, const TextureSet& textures, const std::vector<glm::mat4>* boneTransforms) const
	{
		if (boneTransforms && !m_boneOffsets.empty())
		{
			std::vector<glm::mat4> matrices(boneTransforms->size(), glm::mat4(1.0f));

			for (const BoneOffset& bone : m_boneOffsets)
			{
				matrices[bone.id] =
					(*boneTransforms)[bone.id] * bone.offset;
			}

			shader.SetMatrix4Array("boneMatrices[0]", matrices);
		}

		BindSlot(shader, textures.color, 0,
			"colorTexture", "hasColorTexture");
		BindSlot(shader, textures.normal, 1,
			"normalTexture", "hasNormalTexture");
		BindSlot(shader, textures.occlusion, 2,
			"occlusionTexture", "hasOcclusionTexture");
		/*BindSlot(shader, textures.roughness, 3,
			"roughnessTexture", "hasRoughnessTexture")*/;

		glBindVertexArray(m_vertexArray);
		glDrawElements(GL_TRIANGLES, m_indexCount,
			GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}
