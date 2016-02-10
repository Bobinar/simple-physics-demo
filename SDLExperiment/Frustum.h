#pragma once
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Plane.h"
#include "IntersectionTests.h"

class Frustum
{
	std::vector<Plane*> m_planes;

public:
	Frustum(glm::mat4 mat)
	{
		m_planes.resize(6);
		// from: http://stackoverflow.com/questions/11789766/opengl-glm-calculating-frustum-problems
		
		glm::vec3 planeNormal;
		m_planes[0] = Plane::CreateNormalisedPlane(glm::vec3(
			mat[0][3] + mat[0][0],
			mat[1][3] + mat[1][0],
			mat[2][3] + mat[2][0])
			, mat[3][3] + mat[3][0]);
		
		m_planes[1] = Plane::CreateNormalisedPlane(glm::vec3(
			mat[0][3] - mat[0][0],
			mat[1][3] - mat[1][0],
			mat[2][3] - mat[2][0])
			, mat[3][3] - mat[3][0]);

		m_planes[2] = Plane::CreateNormalisedPlane(glm::vec3(
			mat[0][3] - mat[0][1],
			mat[1][3] - mat[1][1],
			mat[2][3] - mat[2][1])
			, mat[3][3] - mat[3][1]);

		m_planes[3] = Plane::CreateNormalisedPlane(glm::vec3(
			mat[0][3] + mat[0][1],
			mat[1][3] + mat[1][1],
			mat[2][3] + mat[2][1])
			, mat[3][3] + mat[3][1]);

		m_planes[4] = Plane::CreateNormalisedPlane(glm::vec3(
			mat[0][3] + mat[0][2],
			mat[1][3] + mat[1][2],
			mat[2][3] + mat[2][2])
			, mat[3][3] + mat[3][2]);

		m_planes[5] = Plane::CreateNormalisedPlane(glm::vec3(
			mat[0][3] - mat[0][2],
			mat[1][3] - mat[1][2],
			mat[2][3] - mat[2][2])
			, mat[3][3] - mat[3][2]);
	}

	bool IsInFrustum(glm::vec3 &point) const
	{
		int nfails = 0;
		for (int i = 0;i < 6; i++)
		{
			if (IntersectionTests::DistPointPlane(point, *(m_planes[i])) > 0.0f)
			{
				return false;
			}
		}
		return true;
	}
};