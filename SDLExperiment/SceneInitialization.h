#pragma once
#include "SceneRenderer.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Ball.h"
#include "RenderConstants.h"
#include "SceneSimulator.h"
#include "glm/vec3.hpp" 
#include "glm/mat4x4.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/constants.hpp" 

namespace 
{
	void InitializeGLState()
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	void InitializeShadowMapTextureAndFramebuffer(GLuint &depthTexture, GLuint &shadowMapFramebufferName)
	{
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, RenderConstants::ShadowMapResolution, RenderConstants::ShadowMapResolution, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &shadowMapFramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFramebufferName);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	}

	SceneSimulator * CreateSimulator()
	{
		SceneSimulator * pSceneSimulator = new SceneSimulator();

		pSceneSimulator->AddPlane(new Plane(glm::vec3(0, 1, 0), 0));
		pSceneSimulator->AddPlane(new Plane(glm::vec3(0, 0, 1), 0));

		return pSceneSimulator;
	}
}

namespace SceneInitialization
{
	SceneManager* CreateScene(GLuint width, GLuint height)
	{
		InitializeGLState();

		GLuint depthTexture, shadowMapFramebufferName;
		InitializeShadowMapTextureAndFramebuffer(depthTexture, shadowMapFramebufferName);

		Shader* pSceneShader = new Shader(SceneShader::vertexShaderString, SceneShader::fragmentShaderString);
		Shader* pDepthShader = new Shader(ShadowMapShader::vertexShaderString, ShadowMapShader::fragmentShaderString);

		const float BallRadious = 0.1f;
		const glm::vec3 BallStartPosition(0.0f, 1.0f, 0.25f);
		Ball * pBall = new Ball(BallStartPosition, BallRadious, 20, 20);
		Quad * pQuad = new Quad();

		const glm::vec3 CameraPostion(0, 2, 5);
		const glm::vec3 CameraLookAt(0, 0.5f, 0);
		const glm::vec3 CameraUp(0, 1, 0);
		glm::mat4 viewMatrix(glm::lookAt(CameraPostion, CameraLookAt, CameraUp));

		const float FOV = glm::pi<float>() * 0.25f;
		const float NearDistance = 0.1f;
		const float FarDistance = 10.f;
		glm::mat4 projectionMatrix(glm::perspective<float>(FOV, ((float)width) / height, NearDistance, FarDistance));

		glm::vec3 lightPosition(0.25f, 1.65f, 1.0f);

		const float DepthFOV = glm::pi<float>() * 0.35f;
		const float DepthNearDistance = 1.0f;
		const float DepthFarDistance = 3.0f;
		const float DepthAspectRatio = 1.0f;
		glm::mat4 depthProjectionMatrix = glm::perspective<float>(DepthFOV, DepthAspectRatio, DepthNearDistance, DepthFarDistance);

		glm::vec3 lightLookAt(0, 0, 1);
		glm::vec3 lightUp(0, 1, 0);
		glm::mat4 depthViewMatrix = glm::lookAt(lightPosition + glm::vec3(0,1,0), lightLookAt, lightUp);

		glm::mat4 m_lightSpaceViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;

		SceneRenderer * pSceneRenderer = new SceneRenderer(
			width,
			height,
			pSceneShader,
			pDepthShader,
			pQuad,
			viewMatrix,
			projectionMatrix,
			m_lightSpaceViewProjectionMatrix,
			lightPosition,
			depthTexture,
			shadowMapFramebufferName);

		SceneSimulator * pSceneSimulator = CreateSimulator();

		SceneManager * pSceneManager = new SceneManager(pSceneRenderer, pSceneSimulator);

		pSceneManager->AddBall(pBall);

		return pSceneManager;
	}
		
}
