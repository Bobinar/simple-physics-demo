#include <SceneSimulator.h>

#include <IntersectionTests.h>
#include <Sphere.h>

#include <glm/vec3.hpp>

void SceneSimulator::AddPlane(std::unique_ptr<Plane> plane)
{
	m_planes.push_back(std::move(plane));
}

void SceneSimulator::DetectAndResolvePlaneSphereCollision(Plane& plane, Sphere & sphere)
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

void SceneSimulator::DetectAndResolveSphereSphereCollision(Sphere& sphereA, Sphere & sphereB)
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

void SceneSimulator::Update(float deltaTime, std::vector<Sphere*> &spheres)
{
	for (std::vector<Sphere*>::iterator sphereIterator = spheres.begin(); sphereIterator != spheres.end(); ++sphereIterator)
	{
		(*sphereIterator)->Update(deltaTime);
	}

	for (std::vector<Sphere*>::iterator sphereIteratorA = spheres.begin(); sphereIteratorA != spheres.end(); ++sphereIteratorA)
	{
		for (std::vector<Sphere*>::iterator sphereIteratorB = sphereIteratorA + 1; sphereIteratorB != spheres.end(); ++sphereIteratorB)
		{
			DetectAndResolveSphereSphereCollision(**sphereIteratorA, **sphereIteratorB);
		}
	}

	for (std::vector<std::unique_ptr<Plane>>::iterator planeIterator = m_planes.begin(); planeIterator != m_planes.end(); ++planeIterator)
	{
		for (std::vector<Sphere*>::iterator sphereIterator = spheres.begin(); sphereIterator != spheres.end(); ++sphereIterator)
		{
			DetectAndResolvePlaneSphereCollision(**planeIterator, **sphereIterator);
		}
	}
}