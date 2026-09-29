#pragma once

#include <vector>
#include "GameObject.h"

namespace Mira {
	class Scene
	{
		public:
			void AddObject(std::unique_ptr<GameObject> object);
			std::vector<std::unique_ptr<GameObject>>& GetObjects();
			const std::vector<std::unique_ptr<GameObject>>& GetObjects() const;
			bool IsColliding(const GameObject& object) const;

		private:
			std::vector<std::unique_ptr<GameObject>> m_objects;

	};
}
