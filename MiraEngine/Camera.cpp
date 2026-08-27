#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Wave {
	Camera::Camera(const glm::vec3& position, const glm::vec3& target) : m_position(position), m_target(target)
	{
		
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(m_position, m_target, m_up);
	}

	glm::mat4 Camera::GetProjectMatrix(float aspectRatio) const
	{
		return glm::perspective(glm::radians(m_fov), aspectRatio, m_nearPlane, m_farPlane);
	}
}
