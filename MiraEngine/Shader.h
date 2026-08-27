#pragma once

#include <glm/mat4x4.hpp>

namespace Wave
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

	private:
		unsigned int Compile(unsigned int type, const char* source);
		unsigned int m_program = 0;
	};

}


