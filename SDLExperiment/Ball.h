#pragma once
#include "glm/vec3.hpp" 
#include "SphereMesh.h"

class Ball
{
private:
	SphereMesh m_sphereMesh;
	float m_mass;

public:
	glm::vec3 Speed;
	
	glm::vec3 Position;
	float Radius;

	Ball(const glm::vec3 &position, float radius, unsigned int rings, unsigned int sectors)
		: m_sphereMesh(radius, rings, sectors)
		, Position(position)
		, Radius(radius)
		, m_mass(1.0f)
		, Speed(0,0,0)
	{

	}

	void Update(float deltaTime)
	{
		const float GravityFactor = -9.8f;
		glm::vec3 gravityForce(0, GravityFactor * m_mass, 0);

		glm::vec3 forceSum = gravityForce;
		
		glm::vec3 acceleration = forceSum / m_mass;

		float dampingFactor = 0.999f;
		glm::vec3 newSpeed = Speed + acceleration * deltaTime * dampingFactor;

		glm::vec3 newPosition = Position + newSpeed * deltaTime;

		Speed = newSpeed;
		Position = newPosition;
	}

	void Draw() const
	{
		m_sphereMesh.Draw();
	}
};
