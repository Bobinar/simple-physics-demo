#pragma once
#include "glm/vec3.hpp" 
#include "glm/vec4.hpp" 
#include "glm/mat4x4.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/constants.hpp" 
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
		glm::vec3 gravityForce(0, -9.8 * m_mass, 0);
		
		glm::vec3 forceSum = gravityForce;

		if (Position.y < Radius)
		{
			//const float ReboundFactor = 10.f;
			//forceSum += m_mass * m_speed.length() * glm::vec3(0,1,0) * ReboundFactor;
			if (m_speed.y < 0)
			{
				m_speed = -m_speed *0.9f;
			}
		}
		
		glm::vec3 acceleration = forceSum / m_mass;

		float dampening = 0.999f;
		glm::vec3 newSpeed = m_speed + acceleration * deltaTime * dampening;

		glm::vec3 newPosition = Position + newSpeed * deltaTime;

		m_speed = newSpeed;
		Position = newPosition;
	}

	void Draw() const
	{
		m_sphereMesh.Draw();
	}
};
