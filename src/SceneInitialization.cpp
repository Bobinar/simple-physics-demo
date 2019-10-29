#include <SceneInitialization.h>

#include <Plane.h>
#include <QuadMesh.h>
#include <RenderConstants.h>
#include <SceneManager.h>
#include <SceneRenderer.h>
#include <SceneShader.h>
#include <SceneSimulator.h>
#include <Shader.h>
#include <ShadowMapShader.h>
#include <Sphere.h>
#include <SphereMesh.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <memory>

namespace
{
	void InitializeGLState()
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	void InitializeShadowMapTextureAndFramebuffer(
		GLuint& depthTexture, GLuint& shadowMapFramebufferName)
	{
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
					 RenderConstants::ShadowMapResolution,
					 RenderConstants::ShadowMapResolution, 0, GL_DEPTH_COMPONENT,
					 GL_UNSIGNED_SHORT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &shadowMapFramebufferName);

		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebufferName);

#ifndef __EMSCRIPTEN__
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
#endif
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
							   depthTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << ("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO\n")
					  << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	std::unique_ptr<SceneSimulator> CreateSimulator()
	{
		auto pSceneSimulator = std::make_unique<SceneSimulator>();

		pSceneSimulator->AddPlane(std::make_unique<Plane>(glm::vec3(0, 1, 0), 0.0f));
		pSceneSimulator->AddPlane(std::make_unique<Plane>(glm::vec3(0, 0, 1), 0.0f));

		return pSceneSimulator;
	}
} // namespace

namespace SceneInitialization
{
	SceneManager* CreateScene(GLuint width, GLuint height)
	{
		InitializeGLState();

		GLuint depthTexture, shadowMapFramebufferName;
		InitializeShadowMapTextureAndFramebuffer(depthTexture,
												 shadowMapFramebufferName);

		Shader* pSceneShader = new Shader(SceneShader::vertexShaderString,
										  SceneShader::fragmentShaderString);
		Shader* pDepthShader = new Shader(ShadowMapShader::vertexShaderString,
										  ShadowMapShader::fragmentShaderString);

		const glm::vec3 CameraPostion(0, 2, 5);
		const glm::vec3 CameraLookAt(0, 0.5f, 0);
		const glm::vec3 CameraUp(0, 1, 0);
		glm::mat4 viewMatrix(glm::lookAt(CameraPostion, CameraLookAt, CameraUp));

		const float FOV = glm::pi<float>() * 0.25f;
		const float NearDistance = 1.5f;
		const float FarDistance = 10.f;
		glm::mat4 projectionMatrix(glm::perspective<float>(
			FOV, ((float)width) / height, NearDistance, FarDistance));

		glm::vec3 lightPosition(0.25f, 1.65f, 1.0f);

		const float DepthFOV = glm::pi<float>() * 0.35f;
		const float DepthNearDistance = 1.0f;
		const float DepthFarDistance = 3.0f;
		const float DepthAspectRatio = 1.0f;
		glm::mat4 depthProjectionMatrix = glm::perspective<float>(
			DepthFOV, DepthAspectRatio, DepthNearDistance, DepthFarDistance);

		glm::vec3 lightLookAt(0, 0, 1);
		glm::vec3 lightUp(0, 1, 0);
		glm::mat4 depthViewMatrix =
			glm::lookAt(lightPosition + glm::vec3(0, 1, 0), lightLookAt, lightUp);

		glm::mat4 m_lightSpaceViewProjectionMatrix =
			depthProjectionMatrix * depthViewMatrix;

		const glm::vec3 SphereStartPosition(0.0f, 1.0f, 0.25f);
		Sphere sphere(SphereStartPosition, RenderConstants::SphereRadius);

		const int SphereMeshRingsAndSectors = 20;
		SphereMesh* pSphereMesh =
			new SphereMesh(RenderConstants::SphereRadius, SphereMeshRingsAndSectors,
						   SphereMeshRingsAndSectors);

		const float QuadHalfWidth = 3;
		const float QuadZ = 0;
		QuadMesh* pQuadMesh = new QuadMesh(QuadHalfWidth, QuadZ);

		std::unique_ptr<SceneRenderer> pSceneRenderer =
			std::make_unique<SceneRenderer>(
				width, height, pSceneShader, pDepthShader, pQuadMesh, pSphereMesh,
				viewMatrix, projectionMatrix, m_lightSpaceViewProjectionMatrix,
				lightPosition, depthTexture, shadowMapFramebufferName);

		std::unique_ptr<SceneSimulator> pSceneSimulator = CreateSimulator();

		SceneManager* pSceneManager =
			new SceneManager(std::move(pSceneRenderer), std::move(pSceneSimulator));

		pSceneManager->AddSphere(sphere);

		return pSceneManager;
	}
} // namespace SceneInitialization
