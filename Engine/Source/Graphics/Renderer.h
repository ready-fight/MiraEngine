#pragma once

#include "Shader.h"

namespace Mira {

	class Camera;
	class Scene;
	class Model;
	class Transform;
	class Animator;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void Render(float aspectRatio, Camera& camera, Scene& scene);
		void RenderModel(
			float aspectRatio,
			Camera& camera,
			const Transform& transform,
			const Model& model,
			const Animator& animator
		);

	private:
		Shader m_shader;
	};
}

