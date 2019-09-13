#pragma once
#include <glm/vec3.hpp>

class Plane
{
public:
	//TODO: private
	const glm::vec3 Normal;
	const float D;

	Plane(glm::vec3 normal, float d)
		: Normal(normal)
		, D(d)
	{
	}

	static Plane * CreateNormalisedPlane(glm::vec3 normal, float d)
	{
		float mag = (float) normal.length();
		glm::vec3 normalisedNormal = normal / mag;
		float normalisedD = d / mag;
		return new Plane(normalisedNormal, normalisedD);
	}
};
