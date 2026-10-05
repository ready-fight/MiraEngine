#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>
#include <vector>
#include <memory>

namespace MiraEngine {

	class Texture;
	class Shader;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord{ 0.0f };

		glm::ivec4 boneIDs = glm::ivec4(0);
		glm::vec4 weights = glm::vec4(0.0f);
	};

	class Mesh
	{
		public:
			explicit Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t> &indices, std::shared_ptr<Texture> texture = nullptr);
			~Mesh();

			Mesh(const Mesh&) = delete;
			Mesh& operator=(const Mesh&) = delete;

			void Draw(
				const Shader& shader
			) const;
			Texture* GetTexture() const;
			void SetTexture(
				std::shared_ptr<Texture> texture
			);

		private:
			unsigned int m_vertexArray = 0;
			unsigned int m_vertexBuffer = 0;
			unsigned int m_indexBuffer = 0;
			std::shared_ptr<Texture> m_texture;

			int m_indexCount = 0;
	};
}


