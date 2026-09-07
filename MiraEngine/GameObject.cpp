#include "GameObject.h"


namespace Mira {
	const Model& GameObject::GetModel() const
	{
		return *m_model;
	}

	Transform& GameObject::GetTransform()
	{
		return m_transform;
	}

	const Transform& GameObject::GetTransform() const
	{
		return m_transform;
	}

	const glm::vec3& GameObject::GetColor() const
	{
		return m_color;
	}

	void GameObject::SetColor(glm::vec3& color)
	{
		m_color = color;
	}
}

