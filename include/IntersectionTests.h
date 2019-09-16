#pragma once

#include <Plane.h>

#include <glm/detail/func_geometric.hpp>
#include <glm/vec3.hpp>

namespace IntersectionTests
{
	inline float DistPointPlane(const glm::vec3 &point, const Plane &p)
	{
		return (glm::dot(p.Normal, point) - p.D) / glm::dot(p.Normal, p.Normal);
	}
}

