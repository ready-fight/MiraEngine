#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Mira {

	void Transform::SetPosition(const glm::vec3& position)
	{
		m_position = position;
	}

	void Transform::SetRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
	}

	void Transform::SetScale(const glm::vec3& scale)
	{
		m_scale = scale;
	}

	glm::mat4 Transform::GetMatrix() const
	{
		glm::mat4 matrix(1.0f);
		matrix = glm::translate(matrix, m_position);

		matrix = glm::rotate(matrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		matrix = glm::scale(matrix, m_scale);

		return matrix;
	}
}

