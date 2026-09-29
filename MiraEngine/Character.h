#pragma once

#include "GameObject.h"

namespace Mira {

	class Character : public GameObject
	{
		public:
			Character(const std::shared_ptr<Model>& model, const Transform& transform, glm::vec3 color, const char* name);
	};

}