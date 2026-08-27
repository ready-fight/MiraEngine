#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace Wave {
	class Camera
	{
	public:
		Camera(const glm::vec3& position, const glm::vec3& target);

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectMatrix(float aspectRatio) const;

	private:
		glm::vec3 m_position;
		glm::vec3 m_target;
		glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

		float m_fov = 45.0f;
		float m_nearPlane = 0.1f;
		float m_farPlane = 100.0f;
	};
}



