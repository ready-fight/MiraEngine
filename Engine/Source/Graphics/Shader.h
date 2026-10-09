#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace MiraEngine
{
	class Shader
	{
		public:
			Shader(
				const char* vertexSource,
				const char* fragmentSource
			);

			~Shader();

			Shader(const Shader&) = delete;
			Shader& operator=(const Shader&) = delete;

			void Bind() const;
			void Unbind() const;
			void SetMatrix4(
				const char* name,
				const glm::mat4& matrix
			) const;
			void SetMatrix4Array(
				const char* name,
				const std::vector<glm::mat4>& matrices
			) const;
			void SetVector3(
				const char* name,
				const glm::vec3& vector
			) const;
			void SetVector4(
				const char* name,
				const glm::vec4& vector
			) const;
			void SetInt(
				const char* name,
				int value
			) const;
			unsigned int& GetProgram() {
				return m_program;
			}

		private:
			unsigned int Compile(unsigned int type, const char* source);
			unsigned int m_program = 0;
	};

}


