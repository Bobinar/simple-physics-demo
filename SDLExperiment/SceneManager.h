#pragma once
#include "SceneRenderer.h"
#include "SceneSimulator.h"

class SceneManager
{
	SceneRenderer * m_pSceneRenderer;
	SceneSimulator * m_pSceneSimulator;

public:
	SceneManager(SceneRenderer *pSceneRenderer, SceneSimulator *pSceneSimulator)
		: m_pSceneRenderer(pSceneRenderer)
		, m_pSceneSimulator(pSceneSimulator)
	{
	}

	~SceneManager()
	{
		delete m_pSceneRenderer;
		delete m_pSceneSimulator;
	}

	void Update(float deltaTime)
	{
		m_pSceneSimulator->Update(deltaTime);
	}

	void Draw()
	{
		m_pSceneRenderer->Draw();
	}
};
