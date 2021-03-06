#include <SceneRenderer.h>

#include <QuadMesh.h>
#include <RenderConstants.h>
#include <Shader.h>
#include <Sphere.h>
#include <SphereMesh.h>


void SceneRenderer::DrawWalls(const Shader& shader) const
{
	// Wall only needs brought up 1.0 (to sit on the origin)
	glm::mat4 wallModelMatrix = glm::translate(glm::mat4(), glm::vec3(0, 1, 0));
	Draw(shader, wallModelMatrix, *m_pQuadMesh);

	glm::mat4 floorModelTranslationMatrix =
		glm::translate(glm::mat4(), glm::vec3(0, 0, 1));
	glm::mat4 floorModelRotationMatrix =
		glm::rotate(glm::mat4(), glm::pi<float>() * -0.5f, glm::vec3(1, 0, 0));
	glm::mat4 onGroundTransform =
		floorModelTranslationMatrix * floorModelRotationMatrix;
	Draw(shader, onGroundTransform, *m_pQuadMesh);
}

SceneRenderer::SceneRenderer(
	GLuint width, GLuint height, std::unique_ptr<Shader>&& pSceneShader,
	std::unique_ptr<Shader>&& pDepthShader, std::unique_ptr<QuadMesh>&& pQuadMesh, std::unique_ptr<SphereMesh>&& pSphereMesh,
	const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix,
	const glm::mat4& lightSpaceViewProjectionMatrix,
	const glm::vec3& lightPosition, GLuint depthTexture,
	GLuint shadowMapFramebufferName)
	: m_width(width), m_height(height), m_pSceneShader(std::move(pSceneShader)),
	  m_pQuadMesh(std::move(pQuadMesh)), m_pSphereMesh(std::move(pSphereMesh)),
	  m_pDepthShader(std::move(pDepthShader)), m_viewMatrix(viewMatrix),
	  m_projectionMatrix(projectionMatrix),
	  m_lightSpaceViewProjectionMatrix(lightSpaceViewProjectionMatrix),
	  m_lightPosition(lightPosition), m_depthTexture(depthTexture),
	  m_shadowMapFramebufferName(shadowMapFramebufferName),
	  m_frustum(projectionMatrix * viewMatrix)
{
	m_shadowMapID =
		glGetUniformLocation(m_pSceneShader->ProgramObject, "shadowMap");
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::Draw(const Shader& shader, glm::mat4 modelMatrix,
						 Drawable& pDrawable) const
{
	glm::mat4 MV = m_viewMatrix * modelMatrix;
	glm::mat4 MVP = m_projectionMatrix * MV;
	glm::mat4 lightMVP = m_lightSpaceViewProjectionMatrix * modelMatrix;

	GLuint MVPMatrixId = glGetUniformLocation(shader.ProgramObject, "MVP");
	GLuint MVMatrixId = glGetUniformLocation(shader.ProgramObject, "M");
	GLuint lightMVPMatrixId =
		glGetUniformLocation(shader.ProgramObject, "lightMVP");

	glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(MVMatrixId, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(lightMVPMatrixId, 1, GL_FALSE, &lightMVP[0][0]);

	pDrawable.Draw();
}

void SceneRenderer::Draw(std::vector<Sphere>& spheres) const
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

	GLuint worldLightPositionUniform =
		glGetUniformLocation(m_pSceneShader->ProgramObject, "worldLightPosition");

	glUniform3fv(worldLightPositionUniform, 1, glm::value_ptr(m_lightPosition));

	DrawWalls(*m_pSceneShader);

	for (std::vector<Sphere>::iterator it = spheres.begin(); it != spheres.end();
		 ++it)
	{
		glm::mat4 sphereModelMatrix = glm::translate(glm::mat4(), it->Position);
		Draw(*m_pSceneShader, sphereModelMatrix, *m_pSphereMesh);
	}
}

void SceneRenderer::DrawShadowMap(std::vector<Sphere>& spheres) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFramebufferName);
	glViewport(0, 0, RenderConstants::ShadowMapResolution,
			   RenderConstants::ShadowMapResolution);
	glDisable(GL_CULL_FACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_pDepthShader->ProgramObject);
	GLuint MVPMatrixId =
		glGetUniformLocation(m_pDepthShader->ProgramObject, "MVP");

	for (std::vector<Sphere>::iterator it = spheres.begin(); it != spheres.end();
		 ++it)
	{
		glm::mat4 sphereModelMatrix = glm::translate(glm::mat4(), it->Position);
		glm::mat4 depthMVP = m_lightSpaceViewProjectionMatrix * sphereModelMatrix;
		glUniformMatrix4fv(MVPMatrixId, 1, GL_FALSE, &depthMVP[0][0]);
		m_pSphereMesh->Draw();
	}

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

glm::vec3 SceneRenderer::UnprojectScreenCoordinateAt(int x, int y)
{
	const float SimulationSpaceDepth = 0.50f;
	glm::vec3 screenCoordinatesWithInvertedY(x, m_height - y,
											 SimulationSpaceDepth);

	glm::vec4 viewport(0, 0, m_width, m_height);
	glm::vec3 unprojectedCoords =
		glm::unProject(screenCoordinatesWithInvertedY, m_viewMatrix,
					   m_projectionMatrix, viewport);
	return unprojectedCoords;
}

bool SceneRenderer::IsInFrustum(glm::vec3& position)
{
	return m_frustum.IsInFrustum(position);
}