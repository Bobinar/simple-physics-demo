#pragma once
#include <Plane.h>

#include <vector>
#include <memory>

class Sphere;

class SceneSimulator
{
private:
	std::vector<Plane> m_planes;

public:
	void AddPlane(const Plane& plane);
	void DetectAndResolvePlaneSphereCollision(const Plane& plane, Sphere& sphere);
	void DetectAndResolveSphereSphereCollision(Sphere& sphereA, Sphere& sphereB);
	void Update(float deltaTime, std::vector<Sphere>& spheres);
};