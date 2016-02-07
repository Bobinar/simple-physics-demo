#pragma once
#include <vector>
#include "SceneRenderer.h"
#include "SceneSimulator.h"

class SceneManager
{
	SceneRenderer * m_pSceneRenderer;
	SceneSimulator * m_pSceneSimulator;
	std::vector<Ball*> m_balls;

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
		
		for (std::vector<Ball *>::iterator it = m_balls.begin(); it != m_balls.end(); ++it)
		{
			delete (*it);
		}
	}

	void Update(float deltaTime)
	{
		m_pSceneSimulator->Update(deltaTime, m_balls);
	}

	void Draw()
	{
		m_pSceneRenderer->Draw(m_balls);
	}

	void AddBall(Ball * pBall)
	{
		m_balls.push_back(pBall);
	}

	void ShootSphere(int x, int y)
	{
		if (m_balls.size() > 0)
		{
			Ball * pBall = *(m_balls.begin());

			pBall->Position = glm::vec3(0, 1, 2);
			pBall->Speed = glm::vec3(0, 0, -1);
		}
	}
};
