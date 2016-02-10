#pragma once
#include "glm/vec3.hpp" 
#include "Plane.h"

namespace IntersectionTests
{
	float DistPointPlane(const glm::vec3 &point, const Plane &p)
	{
		return (glm::dot(p.Normal, point) - p.D) / glm::dot(p.Normal, p.Normal);
	}
}

