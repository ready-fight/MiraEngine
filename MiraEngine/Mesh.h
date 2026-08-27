#pragma once

#include <glm/vec3.hpp>

#include <cstdint>
#include <vector>

namespace Wave {

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

	class Mesh
	{
	public:
		explicit Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t> &indices);
		~Mesh();

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		void Draw() const;

	private:
		unsigned int m_vertexArray = 0;
		unsigned int m_vertexBuffer = 0;
		unsigned int m_indexBuffer = 0;

		int m_indexCount = 0;
	};
}


