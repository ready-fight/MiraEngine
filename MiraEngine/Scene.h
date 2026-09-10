#pragma once

#include <vector>
#include "GameObject.h"

namespace Mira {
	class Scene
	{
		public:
			void AddObject(GameObject object);
			std::vector<GameObject>& GetObjects();
			const std::vector<GameObject>& GetObjects() const;

		private:
			std::vector<GameObject> m_objects;

	};
}
