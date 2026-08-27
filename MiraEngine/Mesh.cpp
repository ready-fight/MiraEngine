#include "Mesh.h"

#include <GL/glew.h>

#include <cstddef>

namespace Wave {
	Mesh::Mesh(
		const std::vector<Vertex>& vertices, 
		const std::vector<std::uint32_t>& indices
	): 
		m_indexCount(static_cast<int>(indices.size()))
	{
		glGenVertexArrays(1, &m_vertexArray);
		glGenBuffers(1, &m_vertexBuffer);
		glGenBuffers(1, &m_indexBuffer);

		glBindVertexArray(m_vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		glBufferData(
			GL_ARRAY_BUFFER, 
			vertices.size() * sizeof(Vertex), 
			vertices.data(), 
			GL_STATIC_DRAW
		);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(std::uint32_t),
			indices.data(),
			GL_STATIC_DRAW
		);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, position))
		);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, normal))
		);

		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &m_indexBuffer);
		glDeleteBuffers(1, &m_vertexBuffer);
		glDeleteVertexArrays(1, &m_vertexArray);
	}
	void Mesh::Draw() const
	{
		glBindVertexArray(m_vertexArray);

		glDrawElements(
			GL_TRIANGLES, 
			m_indexCount, 
			GL_UNSIGNED_INT, 
			nullptr);

		glBindVertexArray(0);
	}
}
