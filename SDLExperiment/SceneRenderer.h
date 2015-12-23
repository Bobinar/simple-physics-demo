#pragma once
#include <gl/glew.h>
#include "glm/vec3.hpp" 
#include "glm/vec4.hpp" 
#include "glm/mat4x4.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/constants.hpp" 
#include <glm/gtc/type_ptr.hpp>
#include "Quad.h"
#include "Shader.h"
#include "Ball.h"
#include "SceneShader.h"
#include "ShadowMapShader.h"


class SceneRenderer
{
private:
	GLuint m_width, m_height;
	Shader m_sceneShader;
	Shader m_depthShader;
	const glm::mat4 m_viewMatrix, m_projectionMatrix;
	Quad m_quad;
	Ball m_sphere;
	glm::vec3 m_lightPosition;
	GLuint m_depthTexture;
	GLuint m_shadowMapFramebufferName;
public:

	SceneRenderer(GLuint width, GLuint height)
		: m_width(width)
		, m_height(height)
		, m_sceneShader(SceneShader::vShaderStr, SceneShader::fShaderStr)
		, m_depthShader(ShadowMapShader::vShaderStr, ShadowMapShader::fShaderStr)
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

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		
		glGenFramebuffers(1, &m_shadowMapFramebufferName);
		
		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		
		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void DrawSphere(Shader &shader, glm::mat4 modelMatrix)
	{
		glm::mat4 MV = m_viewMatrix * modelMatrix;
		glm::mat4 MVP = m_projectionMatrix * MV;

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MVPMatrixId = glGetUniformLocation(shader.ProgramObject, "MVP");
		GLuint MVMatrixId = glGetUniformLocation(shader.ProgramObject, "M");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MVMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);

		m_sphere.Draw();
	}

	void DrawQuad(Shader &shader, glm::mat4 modelMatrix)
	{
		glm::mat4 MV = m_viewMatrix * modelMatrix;
		glm::mat4 MVP = m_projectionMatrix * MV;

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MVPMatrixId = glGetUniformLocation(shader.ProgramObject, "MVP");
		GLuint MVMatrixId = glGetUniformLocation(shader.ProgramObject, "M");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MVMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);

		m_quad.Draw();
	}

	void Draw()
	{
		DrawShadowMap();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
		glViewport(0, 0, m_width, m_height);

		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Shader &shader = m_depthShader;
		Shader &shader = m_sceneShader;
		// Use the program object
		glUseProgram(shader.ProgramObject);

		glm::vec4 lightDirection(0.f, 1.f, 0.5f, 0.f);
		lightDirection = glm::normalize(lightDirection);
		GLuint worldLightDirectionUniform = glGetUniformLocation(m_sceneShader.ProgramObject, "worldLightDirection");
		glUniform3f(worldLightDirectionUniform, lightDirection.x, lightDirection.y, lightDirection.z);

		GLuint worldLightPositionUniform = glGetUniformLocation(m_sceneShader.ProgramObject, "worldLightPosition");
		
		glUniform3fv(worldLightPositionUniform, 1, glm::value_ptr(m_lightPosition));

		// Wall only needs brought up 1.0 (to sit on the origin)
		glm::mat4 wallModelMatrix = glm::translate(glm::mat4(), glm::vec3(0, 1, 0));
		DrawQuad(shader, wallModelMatrix);

		glm::mat4 floorModelTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 1));
		glm::mat4 floorModelRotationMatrix = glm::rotate(glm::mat4(), glm::pi<float>() * -0.5f, glm::vec3(1, 0, 0));
		glm::mat4 onGroundTransform = floorModelTranslationMatrix  * floorModelRotationMatrix;
		DrawQuad(shader, onGroundTransform);
		
		glm::mat4 sphereScaleMatrix;// = glm::scale(glm::mat4(), glm::vec3(m_sphere.Radius));
		glm::mat4 sphereTranslationMatrix = glm::translate(glm::mat4(), m_sphere.Position);
		glm::mat4 sphereModelMatrix = sphereTranslationMatrix * sphereScaleMatrix;
		DrawSphere(shader, sphereModelMatrix);
	}

	void DrawShadowMap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFramebufferName);
		glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		glDisable(GL_CULL_FACE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);

		glDrawBuffer(GL_NONE); // No color buffer is drawn to.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Always check that our framebuffer is ok
		// TODO: Uncomment when drawing something
		//assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
	}

	void Update(float deltaTime)
	{
		m_sphere.Update(deltaTime);
	}
};
