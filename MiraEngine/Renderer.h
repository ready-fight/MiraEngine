#pragma once

#include "Shader.h"

namespace Mira {

	class Camera;
	class Scene;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void Render(float aspectRatio, Camera& camera, Scene& scene);

	private:
		Shader m_shader;
	};
}

