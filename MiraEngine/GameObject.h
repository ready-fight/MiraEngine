#pragma once
#include "Model.h"
#include "Transform.h"

namespace Mira {
	class GameObject
	{
		public:
			GameObject(const std::shared_ptr<Model>& model, const Transform& transform) : m_model(model), m_transform(transform)
			{
			}
	
			const Model& GetModel() const;
			Transform& GetTransform();
			const Transform& GetTransform() const;

		private:
			std::shared_ptr<Model> m_model;
			Transform m_transform;
	};
}

