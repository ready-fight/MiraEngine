#include "Scene.h"

namespace Mira {

	void Scene::AddObject(std::unique_ptr<GameObject> object) {
		m_objects.push_back(std::move(object));
	}


	std::vector<std::unique_ptr<GameObject>>& Scene::GetObjects()
	{
		return m_objects;
	}

	const std::vector<std::unique_ptr<GameObject>>& Scene::GetObjects() const
	{
		return m_objects;
	}
	bool Scene::IsColliding(const GameObject& object) const
	{
		const float objectRadius = object.GetCollisionRadius();

		if(objectRadius <= 0.0f)
		{
			return false; // No collision if the object has no collision radius
		}

		const glm::vec3& objectPosition = object.GetTransform().GetPosition();

		for (const auto& otherPointer : m_objects)
		{
			const GameObject& other = *otherPointer;

			if (&other == &object)
			{
				continue;
			}

			if (other.GetCollisionRadius() <= 0.0f)
			{
				continue;
			}
		}

		return false;
	}
}

