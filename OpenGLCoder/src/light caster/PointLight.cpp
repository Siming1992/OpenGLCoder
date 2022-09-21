#include "PointLight.h"

namespace light {
	PointLight::PointLight(glm::vec3 lightColor, glm::vec3 position, glm::vec3 direction)
		:Light(lightColor,position,direction)
	{
	}

	PointLight::~PointLight()
	{
	}

}