#pragma once
#include <vector>
#include "glm/vec3.hpp" 
#include "Plane.h"

namespace
{
	float DistPointPlane(const glm::vec3 &point, const Plane &p)
	{
		return (glm::dot(p.Normal, point) - p.D) / glm::dot(p.Normal, p.Normal);
	}
}


class SceneSimulator
{
private:
	std::vector<Plane*> m_planes;
	std::vector<Ball*> m_spheres;

public:
	SceneSimulator()
	{

	}

	~SceneSimulator()
	{
	}

	void AddPlane(Plane * plane)
	{
		m_planes.push_back(plane);
	}

	void AddSphere(Ball * sphere)
	{
		m_spheres.push_back(sphere);
	}
	
	void DetectAndResolvePlaneSphereCollision(Plane& plane, Ball & sphere)
	{
		float distanceToPlane =  DistPointPlane(sphere.Position, plane);
		if (distanceToPlane > sphere.Radius)
			return;

		float speedDotNormal = glm::dot(sphere.Speed, plane.Normal);

		if (speedDotNormal < 0)
		{
			const float ReboundEnergyLossFactor = 0.95f;
			sphere.Speed += -speedDotNormal * plane.Normal * 2.0f * ReboundEnergyLossFactor;
		}

		float penetrationDistance = sphere.Radius - distanceToPlane;
		sphere.Position += penetrationDistance * plane.Normal;		

		//TODO: friction
	}

	void Update(float deltaTime)
	{
		for (std::vector<Ball*>::iterator it = m_spheres.begin(); it != m_spheres.end(); ++it)
		{
			(*it)->Update(deltaTime);
		}

		for (std::vector<Plane*>::iterator itA = m_planes.begin(); itA != m_planes.end(); ++itA)
		{
			for (std::vector<Ball*>::iterator itB = m_spheres.begin(); itB != m_spheres.end(); ++itB)
			{
				DetectAndResolvePlaneSphereCollision(**itA, **itB);
			}
		}
	}
};