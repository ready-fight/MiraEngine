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
}

