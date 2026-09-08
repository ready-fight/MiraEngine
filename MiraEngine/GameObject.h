#pragma once
#include "Model.h"
#include "Transform.h"

namespace Mira {
	class GameObject
	{
		public:
			GameObject(const std::shared_ptr<Model>& model) : m_model(model)
			{
			}
			GameObject(const std::shared_ptr<Model>& model, const Transform& transform) : m_model(model), m_transform(transform)
			{
			}
			GameObject(const std::shared_ptr<Model>& model, const Transform& transform, glm::vec3 color) : m_model(model), m_transform(transform), m_color(color)
			{
			}
	
			const Model& GetModel() const;
			Transform& GetTransform();
			const Transform& GetTransform() const;
			const glm::vec3& GetColor() const;
			void SetColor(glm::vec3& color);

		private:
			std::shared_ptr<Model> m_model;
			Transform m_transform;
			glm::vec3 m_color{1.0f};
	};
}

