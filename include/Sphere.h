#pragma once
#include <glm/vec3.hpp>

class Sphere
{
private:
	float m_mass;

public:
	//TODO: private
	glm::vec3 Speed;
	glm::vec3 Position;
	float Radius;

	Sphere(const glm::vec3& position, float radius);
	void Update(float deltaTime);
};
