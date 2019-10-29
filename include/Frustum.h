#pragma once

#include <glm/mat4x4.hpp>

#include <vector>

class Plane;

class Frustum
{
	std::vector<Plane*> m_planes;

public:
	Frustum(glm::mat4 mat);
	bool IsInFrustum(glm::vec3& point) const;
};