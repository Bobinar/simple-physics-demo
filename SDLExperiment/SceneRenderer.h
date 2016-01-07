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
	const GLuint m_width, m_height;
	const Shader m_sceneShader;
	const Shader m_depthShader;
	const glm::mat4 m_viewMatrix, m_projectionMatrix;
	Quad m_quad;
	Ball m_sphere;
	const glm::vec3 m_lightPosition;
	GLuint m_depthTexture;
	GLuint m_shadowMapFramebufferName;
	glm::mat4 m_lightSpaceViewProjectionMatrix;
public:

	SceneRenderer(GLuint width, GLuint height)
		: m_width(width)
		, m_height(height)
		, m_sceneShader(SceneShader::vShaderStr, SceneShader::fShaderStr)
		, m_depthShader(ShadowMapShader::vShaderStr, ShadowMapShader::fShaderStr)
		, m_viewMatrix(glm::lookAt(glm::vec3(0, 3, 5.f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)))
		, m_projectionMatrix(glm::perspective<float>(glm::pi<float>() * 0.25f,((float) m_width )/ m_height, 0.1f, 100.f))
		, m_sphere(glm::vec3(0.0f, 1.5f, 0.2f), 0.1f,20,20)
		, m_lightPosition(0.25f, 1.5f, 1.0f)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);

		glGenFramebuffers(1, &m_shadowMapFramebufferName);
	
		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		glGenTextures(1, &m_depthTexture);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		const GLuint ShadowMapResolution = 1024;
		glewGetExtension("OES_texture_float");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ShadowMapResolution, ShadowMapResolution, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 0.5f, 5.0f);
		glm::mat4 depthViewMatrix = glm::lookAt(m_lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_lightSpaceViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFramebufferName);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_depthTexture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DrawSphere(const Shader &shader, glm::mat4 modelMatrix) const
	{
		glm::mat4 MV = m_viewMatrix * modelMatrix;
		glm::mat4 MVP = m_projectionMatrix * MV;
		glm::mat4 lightMVP = m_lightSpaceViewProjectionMatrix * modelMatrix;

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MVPMatrixId = glGetUniformLocation(shader.ProgramObject, "MVP");
		GLuint MVMatrixId = glGetUniformLocation(shader.ProgramObject, "M");
		GLuint lightMVPMatrixId = glGetUniformLocation(shader.ProgramObject, "lightMVP");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MVMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(lightMVPMatrixId, 1, GL_FALSE, &lightMVP[0][0]);

		m_sphere.Draw();
	}

	void DrawQuad(const Shader &shader, glm::mat4 modelMatrix) const 
	{
		glm::mat4 MV = m_viewMatrix * modelMatrix;
		glm::mat4 MVP = m_projectionMatrix * MV;
		glm::mat4 lightMVP = m_lightSpaceViewProjectionMatrix * modelMatrix;

		// Get a handle for our "MVP" uniform
		// Only during the initialisation
		GLuint MVPMatrixId = glGetUniformLocation(shader.ProgramObject, "MVP");
		GLuint MMatrixId = glGetUniformLocation(shader.ProgramObject, "M");
		GLuint lightMVPMatrixId = glGetUniformLocation(shader.ProgramObject, "lightMVP");

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(lightMVPMatrixId, 1, GL_FALSE, &lightMVP[0][0]);

		m_quad.Draw();
	}

	void Draw() const
	{
		DrawShadowMap();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); 
		glViewport(0, 0, m_width, m_height);

		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		const Shader &shader = m_sceneShader;

		// Use the program object
		glUseProgram(shader.ProgramObject);

		GLuint ShadowMapID = glGetUniformLocation(shader.ProgramObject, "shadowMap");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glUniform1i(ShadowMapID, 0);

		GLuint worldLightPositionUniform = glGetUniformLocation(m_sceneShader.ProgramObject, "worldLightPosition");
		
		glUniform3fv(worldLightPositionUniform, 1, glm::value_ptr(m_lightPosition));

		// Wall only needs brought up 1.0 (to sit on the origin)
		glm::mat4 wallModelMatrix = glm::translate(glm::mat4(), glm::vec3(0, 1, 0));
		DrawQuad(shader, wallModelMatrix);

		glm::mat4 floorModelTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 1));
		glm::mat4 floorModelRotationMatrix = glm::rotate(glm::mat4(), glm::pi<float>() * -0.5f, glm::vec3(1, 0, 0));
		glm::mat4 onGroundTransform = floorModelTranslationMatrix  * floorModelRotationMatrix;
		DrawQuad(shader, onGroundTransform);
		
		glm::mat4 sphereModelMatrix = glm::translate(glm::mat4(), m_sphere.Position);
		DrawSphere(shader, sphereModelMatrix);
	}

	void DrawShadowMap() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFramebufferName);
		glViewport(0, 0, 1024, 1024); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		glDisable(GL_CULL_FACE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const Shader &shader = m_depthShader;

		glUseProgram(shader.ProgramObject);

		glm::mat4 sphereModelMatrix = glm::translate(glm::mat4(), m_sphere.Position);
		glm::mat4 depthMVP = m_lightSpaceViewProjectionMatrix * sphereModelMatrix;

		GLuint MVPMatrixId = glGetUniformLocation(shader.ProgramObject, "MVP");
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &depthMVP[0][0]);
		m_sphere.Draw();

		// Always check that our framebuffer is ok
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void Update(float deltaTime)
	{
		m_sphere.Update(deltaTime);
	}
};
