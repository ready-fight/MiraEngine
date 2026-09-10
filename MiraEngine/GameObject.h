#pragma once
#include "Model.h"
#include "Transform.h"

namespace Mira {
	class GameObject
	{
		public:
			GameObject(const std::shared_ptr<Model>& model, const Transform& transform, glm::vec3 color, const char* name) : m_model(model), m_transform(transform), m_color(color), m_name(name)
			{
			}
	
			const Model& GetModel() const { return *m_model; }
			Transform& GetTransform() { return m_transform; }
			const Transform& GetTransform() const { return m_transform; }
			const glm::vec3& GetColor() const { return m_color; }
			const std::string& GetName() const { return m_name; }

			void SetName(const std::string& name) { m_name = name; }
			void SetColor(const glm::vec3& color) { m_color = color; }

		private:
			std::shared_ptr<Model> m_model;
			Transform m_transform;
			glm::vec3 m_color{1.0f};
			std::string m_name;
	};
}

