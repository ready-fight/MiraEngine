#pragma once

#include "Model.h"
#include "Shader.h"

namespace Wave {

	class Camera;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void Render(float aspectRatio, Camera& camera);

	private:
		Model m_model;
		Shader m_shader;
	};
}

