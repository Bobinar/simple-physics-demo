#pragma once
#include <gl/glew.h>
#include "glm/vec3.hpp" 
#include "glm/vec4.hpp" 
#include "glm/mat4x4.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/constants.hpp" 
#include <glm/gtc/type_ptr.hpp>
#include "Quad.h"
#include "SampleShader.h"
#include "SphereMesh.h"
#include "Ball.h"


class SceneRenderer
{
private:
	GLuint m_width, m_height;
	SampleShader m_shader;
	const glm::mat4 m_viewMatrix, m_projectionMatrix;
	Quad m_quad;
	Ball m_sphere;
	glm::vec3 m_lightPosition;
public:

	SceneRenderer(GLuint width, GLuint height)
		: m_width(width)
		, m_height(height)
		, m_shader()
		, m_viewMatrix(glm::lookAt(glm::vec3(0, 3, 5.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)))
		, m_projectionMatrix(glm::perspective<float>(glm::pi<float>() * 0.25f,((float) m_width )/ m_height, 0.1f, 100.f))
		, m_sphere(glm::vec3(0.0f, 2.0f, 1.0f), 0.1f,20,20)
		, m_lightPosition(0.25f, 1.5f, 1.0f)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

	void DrawSphere(glm::mat4 modelMatrix)
	{
		glm::mat4 MV = m_viewMatrix * modelMatrix;
		glm::mat4 MVP = m_projectionMatrix * MV;

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MVPMatrixId = glGetUniformLocation(m_shader.ProgramObject, "MVP");
		GLuint MVMatrixId = glGetUniformLocation(m_shader.ProgramObject, "M");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MVMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);

		m_sphere.Draw();
	}

	void DrawQuad(glm::mat4 modelMatrix)
	{
		glm::mat4 MV = m_viewMatrix * modelMatrix;
		glm::mat4 MVP = m_projectionMatrix * MV;

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MVPMatrixId = glGetUniformLocation(m_shader.ProgramObject, "MVP");
		GLuint MVMatrixId = glGetUniformLocation(m_shader.ProgramObject, "M");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MVMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);

		m_quad.Draw();
	}

	void Draw()
	{
		glViewport(0, 0, m_width, m_height);

		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the program object
		glUseProgram(m_shader.ProgramObject);

		glm::vec4 lightDirection(0.f, 1.f, 0.5f, 0.f);
		lightDirection = glm::normalize(lightDirection);
		GLuint worldLightDirectionUniform = glGetUniformLocation(m_shader.ProgramObject, "worldLightDirection");
		glUniform3f(worldLightDirectionUniform, lightDirection.x, lightDirection.y, lightDirection.z);

		GLuint worldLightPositionUniform = glGetUniformLocation(m_shader.ProgramObject, "worldLightPosition");
		
		glUniform3fv(worldLightPositionUniform, 1, glm::value_ptr(m_lightPosition));

		// Wall only needs brought up 1.0 (to sit on the origin)
		glm::mat4 wallModelMatrix = glm::translate(glm::mat4(), glm::vec3(0, 1, 0));
		DrawQuad(wallModelMatrix);

		glm::mat4 floorModelTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 1));
		glm::mat4 floorModelRotationMatrix = glm::rotate(glm::mat4(), glm::pi<float>() * -0.5f, glm::vec3(1, 0, 0));
		glm::mat4 onGroundTransform = floorModelTranslationMatrix  * floorModelRotationMatrix;
		DrawQuad(onGroundTransform);
		
		glm::mat4 sphereScaleMatrix;// = glm::scale(glm::mat4(), glm::vec3(m_sphere.Radius));
		glm::mat4 sphereTranslationMatrix = glm::translate(glm::mat4(), m_sphere.Position);
		glm::mat4 sphereModelMatrix = sphereTranslationMatrix * sphereScaleMatrix;
		DrawSphere(sphereModelMatrix);
	}

	void Update(float deltaTime)
	{
		m_sphere.Update(deltaTime);
	}
	
};
