#pragma once
#include <gl/glew.h>
#include "glm/vec3.hpp" 
#include "glm/mat4x4.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/constants.hpp" 
#include <glm/gtc/type_ptr.hpp>
#include "QuadMesh.h"
#include "Shader.h"
#include "Ball.h"
#include "SceneShader.h"
#include "ShadowMapShader.h"

class SceneRenderer
{
private:
	const GLuint m_width, m_height;
	const Shader* m_pSceneShader;
	const Shader* m_pDepthShader;
	const glm::mat4 m_viewMatrix, m_projectionMatrix;
	QuadMesh* m_pQuadMesh;
	SphereMesh* m_pSphereMesh;
	
	const glm::vec3 m_lightPosition;
	GLuint m_depthTexture;
	GLuint m_shadowMapFramebufferName;
	glm::mat4 m_lightSpaceViewProjectionMatrix;
	GLuint m_shadowMapID;

	void DrawWalls(const Shader &shader) const
	{
		// Wall only needs brought up 1.0 (to sit on the origin)
		glm::mat4 wallModelMatrix = glm::translate(glm::mat4(), glm::vec3(0, 1, 0));
		Draw(shader, wallModelMatrix, m_pQuadMesh);

		glm::mat4 floorModelTranslationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 1));
		glm::mat4 floorModelRotationMatrix = glm::rotate(glm::mat4(), glm::pi<float>() * -0.5f, glm::vec3(1, 0, 0));
		glm::mat4 onGroundTransform = floorModelTranslationMatrix  * floorModelRotationMatrix;
		Draw(shader, onGroundTransform, m_pQuadMesh);
	}

public:

	SceneRenderer(
		GLuint width,
		GLuint height,
		const Shader* pSceneShader,
		const Shader* pDepthShader,
		QuadMesh * pQuadMesh,
		SphereMesh * pSphereMesh,
		const glm::mat4 &viewMatrix,
		const glm::mat4 &projectionMatrix,
		const glm::mat4 &lightSpaceViewProjectionMatrix,
		const glm::vec3 &lightPosition,
		GLuint depthTexture, 
		GLuint shadowMapFramebufferName)
		: m_width(width)
		, m_height(height)
		, m_pSceneShader(pSceneShader)
		, m_pDepthShader(pDepthShader)
		, m_viewMatrix(viewMatrix)
		, m_projectionMatrix(projectionMatrix)
		, m_lightSpaceViewProjectionMatrix(lightSpaceViewProjectionMatrix)
		, m_pQuadMesh(pQuadMesh)
		, m_pSphereMesh(pSphereMesh)
		, m_lightPosition(lightPosition)
		, m_depthTexture(depthTexture)
		, m_shadowMapFramebufferName(shadowMapFramebufferName)
	{
		m_shadowMapID = glGetUniformLocation(m_pSceneShader->ProgramObject, "shadowMap");
	}

	~SceneRenderer()
	{
		delete m_pQuadMesh;
		delete m_pSphereMesh;
		delete m_pSceneShader;
		delete m_pDepthShader;
	}

	void Draw(const Shader &shader, glm::mat4 modelMatrix, Drawable * pDrawable) const
	{
		glm::mat4 MV = m_viewMatrix * modelMatrix;
		glm::mat4 MVP = m_projectionMatrix * MV;
		glm::mat4 lightMVP = m_lightSpaceViewProjectionMatrix * modelMatrix;

		GLuint MVPMatrixId = glGetUniformLocation(shader.ProgramObject, "MVP");
		GLuint MVMatrixId = glGetUniformLocation(shader.ProgramObject, "M");
		GLuint lightMVPMatrixId = glGetUniformLocation(shader.ProgramObject, "lightMVP");

		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(MVMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(lightMVPMatrixId, 1, GL_FALSE, &lightMVP[0][0]);

		pDrawable->Draw();
	}

	void Draw(std::vector<Ball*> &spheres) const
	{
		DrawShadowMap(spheres);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); 
		glViewport(0, 0, m_width, m_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(m_pSceneShader->ProgramObject);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);

		GLuint worldLightPositionUniform = glGetUniformLocation(m_pSceneShader->ProgramObject, "worldLightPosition");
		
		glUniform3fv(worldLightPositionUniform, 1, glm::value_ptr(m_lightPosition));

		DrawWalls(*m_pSceneShader);
		
		for (std::vector<Ball *>::iterator it = spheres.begin(); it != spheres.end(); ++it)
		{
			glm::mat4 sphereModelMatrix = glm::translate(glm::mat4(), (*it)->Position);
			Draw(*m_pSceneShader, sphereModelMatrix, m_pSphereMesh);
		}
	}

	void DrawShadowMap(std::vector<Ball*> &spheres) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFramebufferName);
		glViewport(0, 0, RenderConstants::ShadowMapResolution, RenderConstants::ShadowMapResolution);
		glDisable(GL_CULL_FACE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_pDepthShader->ProgramObject);
		GLuint MVPMatrixId = glGetUniformLocation(m_pDepthShader->ProgramObject, "MVP");

		for (std::vector<Ball *>::iterator it = spheres.begin(); it != spheres.end(); ++it)
		{
			glm::mat4 sphereModelMatrix = glm::translate(glm::mat4(), (*it)->Position);
			glm::mat4 depthMVP = m_lightSpaceViewProjectionMatrix * sphereModelMatrix;
			glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &depthMVP[0][0]);
			m_pSphereMesh->Draw();
		}

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	glm::vec3 UnprojectScreenCoordinateAt(glm::vec3 &screenCoordinates)
	{
		return glm::vec3();
	}
};
