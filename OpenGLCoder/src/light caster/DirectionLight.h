#pragma once

#include "light caster/Light.h"

namespace light {
	class DirectionLight :public Light
	{
	public:
		DirectionLight(glm::vec3 lightColor = glm::vec3(1), glm::vec3 position = glm::vec3(), glm::vec3 direction = glm::vec3());
		~DirectionLight();

		//glm::vec3 Direction;

	private:

	};
}

