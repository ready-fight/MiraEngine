#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/common.hpp>

#include <cmath>
#include <cstdio>

namespace MiraEngine
{
	Camera::Camera(
		const glm::vec3& position,
		const glm::vec3& target
	)
		: m_position(position),
		m_target(target)
	{
		const glm::vec3 direction =
			glm::normalize(
				m_target - m_position
			);

		m_pitch =
			glm::degrees(
				std::asin(direction.y)
			);

		m_yaw =
			glm::degrees(
				std::atan2(
					direction.z,
					direction.x
				)
			);

		UpdateTarget();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(
			m_position,
			m_target,
			m_up
		);
	}

	glm::mat4 Camera::GetProjectionMatrix(
		float aspectRatio
	) const
	{
		return glm::perspective(
			glm::radians(m_fov),
			aspectRatio,
			m_nearPlane,
			m_farPlane
		);
	}

	void Camera::Move(
		const glm::vec3& offset
	)
	{
		m_position += offset;
		m_target += offset;

		printf(
			"Camera Position: (%.2f, %.2f, %.2f)\n",
			m_position.x,
			m_position.y,
			m_position.z
		);
	}

	void Camera::MoveRelative(
		float forward,
		float right,
		float vertical
	)
	{
		const glm::vec3 offset =
			m_forward * forward +
			m_right * right +
			m_up * vertical;

		Move(offset);
	}

	void Camera::Rotate(
		float yawOffset,
		float pitchOffset
	)
	{
		m_yaw += yawOffset;

		m_pitch = glm::clamp(
			m_pitch + pitchOffset,
			-89.0f,
			89.0f
		);

		UpdateTarget();
	}

	void Camera::SetYaw(float yaw)
	{
		m_yaw = yaw;

		UpdateTarget();
	}

	void Camera::SetPitch(float pitch)
	{
		m_pitch = pitch;

		UpdateTarget();
	}

	void Camera::UpdateTarget()
	{
		const float yaw =
			glm::radians(m_yaw);

		const float pitch =
			glm::radians(m_pitch);

		glm::vec3 direction;

		direction.x =
			std::cos(yaw) *
			std::cos(pitch);

		direction.y =
			std::sin(pitch);

		direction.z =
			std::sin(yaw) *
			std::cos(pitch);

		m_forward =
			glm::normalize(direction);

		m_right =
			glm::normalize(
				glm::cross(
					m_forward,
					m_up
				)
			);

		m_target =
			m_position +
			m_forward;
	}

	void Camera::SetPosition(
		const glm::vec3& position
	)
	{
		m_position = position;

		UpdateTarget();
	}

	void Camera::LookAt(
		const glm::vec3& target
	)
	{
		m_target = target;

		const glm::vec3 direction =
			glm::normalize(
				m_target - m_position
			);

		m_pitch =
			glm::degrees(
				std::asin(direction.y)
			);

		m_yaw =
			glm::degrees(
				std::atan2(
					direction.z,
					direction.x
				)
			);

		UpdateTarget();
	}
}