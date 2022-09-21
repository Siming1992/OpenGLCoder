#include "DirectionLight.h"

namespace light {
	DirectionLight::DirectionLight(glm::vec3 lightColor, glm::vec3 position, glm::vec3 direction)
		:Light(lightColor,position,direction)
	{
	}

	DirectionLight::~DirectionLight()
	{

	}
}

