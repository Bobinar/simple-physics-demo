#pragma once
#include <vector>

class Sphere;
class Plane;

class SceneSimulator
{
private:
	std::vector<Plane*> m_planes;

public:
	~SceneSimulator();
	void AddPlane(Plane * plane);
	void DetectAndResolvePlaneSphereCollision(Plane& plane, Sphere & sphere);
	void DetectAndResolveSphereSphereCollision(Sphere& sphereA, Sphere & sphereB);
	void Update(float deltaTime, std::vector<Sphere*> &spheres);
};