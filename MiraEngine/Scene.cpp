#include "Scene.h"

namespace Mira {

	void Scene::AddObject(GameObject object) {

		m_objects.push_back(std::move(object));
	}


	const std::vector<GameObject>& Scene::GetObjects() const
	{
		return m_objects;
	}
}

