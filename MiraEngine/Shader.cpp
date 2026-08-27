#include "Shader.h"

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <string>


namespace Wave {
	Shader::Shader(const char* vertexSource, const char* fragmentSource)
	{
		const unsigned int vertexShader = Compile(GL_VERTEX_SHADER, vertexSource);

		unsigned int fragmentShader;

		try {
			fragmentShader = Compile(GL_FRAGMENT_SHADER, fragmentSource);
		}

		catch (...)
		{
			glDeleteShader(vertexShader);
			throw;
		}

		m_program = glCreateProgram();
		glAttachShader(m_program, vertexShader);
		glAttachShader(m_program, fragmentShader);
		glLinkProgram(m_program);
		int success;
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if (!success)
		{
			char errorMessage[512];
			glGetProgramInfoLog(m_program, sizeof(errorMessage), nullptr, errorMessage);
			glDeleteProgram(m_program);

			throw std::runtime_error(std::string("Shader linking error: ") + errorMessage);
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_program);
	}


	void Shader::Bind() const
	{
		glUseProgram(m_program);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetMatrix4(const char* name, const glm::mat4& matrix) const
	{
		const int location = glGetUniformLocation(m_program, name);

		glUniformMatrix4fv(
			location,
			1,
			GL_FALSE,
			glm::value_ptr(matrix)
		);
	}

	unsigned int Shader::Compile(unsigned int type, const char* source)
	{
		const unsigned int shader = glCreateShader(type);

		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char errorMessage[512];
			glGetShaderInfoLog(shader, sizeof(errorMessage), nullptr, errorMessage);
			glDeleteShader(shader);

			throw std::runtime_error(std::string("Shader compilation failed: ") + errorMessage);
		}

		return shader;
	}

}
