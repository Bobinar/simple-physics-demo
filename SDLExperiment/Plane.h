#pragma once
#include "glm/vec3.hpp" 

class Plane
{
public:
	const glm::vec3 Normal;
	const float D;

	Plane(glm::vec3 normal, float d)
		: Normal(normal)
		, D(d)
	{
	}
};
