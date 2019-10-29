#pragma once

#include <Frustum.h>


#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include <memory>

class Drawable;
class QuadMesh;
class Shader;
class Sphere;
class SphereMesh;

class SceneRenderer
{
private:
	const GLuint m_width, m_height;
	std::unique_ptr<Shader> m_pSceneShader;
	std::unique_ptr<Shader> m_pDepthShader;
	const glm::mat4 m_viewMatrix, m_projectionMatrix;
	std::unique_ptr<QuadMesh> m_pQuadMesh;
	std::unique_ptr<SphereMesh> m_pSphereMesh;
	Frustum m_frustum;

	const glm::vec3 m_lightPosition;
	GLuint m_depthTexture;
	GLuint m_shadowMapFramebufferName;
	glm::mat4 m_lightSpaceViewProjectionMatrix;
	GLuint m_shadowMapID;

	void DrawWalls(const Shader& shader) const;

public:
	SceneRenderer(
		GLuint width,
		GLuint height,
		std::unique_ptr<Shader>&& pSceneShader,
		std::unique_ptr<Shader>&& pDepthShader,
		std::unique_ptr<QuadMesh>&& pQuadMesh,
		std::unique_ptr<SphereMesh>&& pSphereMesh,
		const glm::mat4& viewMatrix,
		const glm::mat4& projectionMatrix,
		const glm::mat4& lightSpaceViewProjectionMatrix,
		const glm::vec3& lightPosition,
		GLuint depthTexture,
		GLuint shadowMapFramebufferName);
	~SceneRenderer();

	void Draw(const Shader& shader, glm::mat4 modelMatrix, Drawable& pDrawable) const;
	void Draw(std::vector<Sphere>& spheres) const;
	void DrawShadowMap(std::vector<Sphere>& spheres) const;
	glm::vec3 UnprojectScreenCoordinateAt(int x, int y);
	bool IsInFrustum(glm::vec3& position);
};
