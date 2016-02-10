#pragma once
#include <vector>
#include "glm/vec3.hpp" 
#include "Plane.h"
#include "IntersectionTests.h"

class SceneSimulator
{
private:
	std::vector<Plane*> m_planes;

public:
	SceneSimulator()
	{

	}

	~SceneSimulator()
	{
		for (std::vector<Plane *>::iterator it = m_planes.begin(); it != m_planes.end(); ++it)
		{
			delete (*it);
		}
	}

	void AddPlane(Plane * plane)
	{
		m_planes.push_back(plane);
	}

	void DetectAndResolvePlaneSphereCollision(Plane& plane, Sphere & sphere)
	{
		float distanceToPlane = IntersectionTests::DistPointPlane(sphere.Position, plane);
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

	void DetectAndResolveSphereSphereCollision(Sphere& sphereA, Sphere & sphereB)
	{
		glm::vec3 distanceVector = sphereA.Position - sphereB.Position;
		float distance = glm::length(distanceVector);

		float radiusSum = sphereA.Radius + sphereB.Radius;

		if (distance > radiusSum)
			return;

		glm::vec3 normalizedDistanceVector = distanceVector / distance;
		glm::vec3 aToBVector = -normalizedDistanceVector;
		glm::vec3 bToAVector = normalizedDistanceVector;

		float penetrationDistance = radiusSum - distance;

		sphereA.Position += penetrationDistance * bToAVector * 0.5f;
		sphereB.Position += penetrationDistance * aToBVector * 0.5f;

		float impactSpeedA = glm::dot(sphereA.Speed, aToBVector);
		float impactSpeedB = glm::dot(sphereB.Speed, bToAVector);

		const float ReboundEnergyLossFactor = 0.95f;
		float speedCorrectionPerSphere = (impactSpeedA + impactSpeedB) * 0.5f * ReboundEnergyLossFactor;
		glm::vec3 speedCorrectionA = (speedCorrectionPerSphere + impactSpeedA) * bToAVector;
		glm::vec3 speedCorrectionB = (speedCorrectionPerSphere + impactSpeedB) * aToBVector;

		sphereA.Speed += speedCorrectionA;
		sphereB.Speed += speedCorrectionB;
	}

	void Update(float deltaTime, std::vector<Sphere*> &spheres)
	{
		for (std::vector<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); ++it)
		{
			(*it)->Update(deltaTime);
		}

		for (std::vector<Sphere*>::iterator itA = spheres.begin(); itA != spheres.end(); ++itA)
		{
			for (std::vector<Sphere*>::iterator itB = itA + 1; itB != spheres.end(); ++itB)
			{
				DetectAndResolveSphereSphereCollision(**itA, **itB);
			}
		}

		for (std::vector<Plane*>::iterator itA = m_planes.begin(); itA != m_planes.end(); ++itA)
		{
			for (std::vector<Sphere*>::iterator itB = spheres.begin(); itB != spheres.end(); ++itB)
			{
				DetectAndResolvePlaneSphereCollision(**itA, **itB);
			}
		}
	}
};