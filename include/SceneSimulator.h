#pragma once
#include <Plane.h>

#include <vector>
#include <memory>

class Sphere;

class SceneSimulator
{
private:
	std::vector<std::unique_ptr<Plane>> m_planes;

public:
	void AddPlane(std::unique_ptr<Plane> plane);
	void DetectAndResolvePlaneSphereCollision(const Plane& plane, Sphere & sphere);
	void DetectAndResolveSphereSphereCollision(Sphere& sphereA, Sphere & sphereB);
	void Update(float deltaTime, std::vector<std::unique_ptr<Sphere>> & spheres);
};