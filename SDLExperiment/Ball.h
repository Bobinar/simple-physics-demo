#pragma once
#include "glm/vec3.hpp" 
#include "SphereMesh.h"

class Ball
{

private:
	glm::vec3 m_speed;
	SphereMesh m_sphereMesh;
	float m_mass;
public:
	
	glm::vec3 Position;
	float Radius;

	Ball(glm::vec3 position, float radius, unsigned int rings, unsigned int sectors)
		: m_sphereMesh(radius, rings, sectors)
		, Position(position)
		, Radius(radius)
		, m_mass(1.0f)
	{

	}

	void Update(float deltaTime)
	{
		const float GravityFactor = -9.8f;
		glm::vec3 gravityForce(0, GravityFactor * m_mass, 0);
		
		glm::vec3 forceSum = gravityForce;

		if (Position.y < Radius)
		{
			if (m_speed.y < 0)
			{
				const float ReboundEnergyLossFactor = 0.95f;
				m_speed = -m_speed * ReboundEnergyLossFactor;
			}
			Position.y = Radius;
		}
		
		glm::vec3 acceleration = forceSum / m_mass;

		float dampingFactor = 0.999f;
		glm::vec3 newSpeed = m_speed + acceleration * deltaTime * dampingFactor;

		glm::vec3 newPosition = Position + newSpeed * deltaTime;

		m_speed = newSpeed;
		Position = newPosition;
	}

	void Draw() const
	{
		m_sphereMesh.Draw();
	}
};
