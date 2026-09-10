#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Mira {
	class Transform
	{

		private:
			glm::vec3 m_position{ 0.0f };
			glm::vec3 m_rotation{ 0.0f };
			glm::vec3 m_scale{ 1.0f };

		public:

			Transform() = default;

			Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : m_position(position), m_rotation(rotation), m_scale(scale)
			{
			}

			const glm::vec3& GetPosition() const { return m_position; }
			const glm::vec3& GetRotation() const { return m_rotation; }
			const glm::vec3& GetScale() const { return m_scale; }
			void SetPosition(const glm::vec3& position);
			void SetRotation(const glm::vec3& rotation);
			void SetScale(const glm::vec3& scale);
			glm::mat4 GetMatrix() const;
	};
}

