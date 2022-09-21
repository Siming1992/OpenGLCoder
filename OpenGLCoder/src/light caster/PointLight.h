#pragma once

#include "Light.h"

namespace light {
	class PointLight:public Light
	{
	public:
		PointLight(glm::vec3 lightColor = glm::vec3(1), glm::vec3 position = glm::vec3(), glm::vec3 direction = glm::vec3());

		~PointLight();
	private:
	};
}

