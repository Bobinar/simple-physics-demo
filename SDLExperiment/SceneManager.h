#pragma once
#include <vector>
#include "SceneRenderer.h"
#include "SceneSimulator.h"

class SceneManager
{
	SceneRenderer * m_pSceneRenderer;
	SceneSimulator * m_pSceneSimulator;
	std::vector<Sphere*> m_spheres;

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
		
		for (std::vector<Sphere *>::iterator it = m_spheres.begin(); it != m_spheres.end(); ++it)
		{
			delete (*it);
		}
	}

	void Update(float deltaTime)
	{
		m_pSceneSimulator->Update(deltaTime, m_spheres);
	}

	void Draw()
	{
		m_pSceneRenderer->Draw(m_spheres);
	}

	void AddSphere(Sphere * pSphere)
	{
		m_spheres.push_back(pSphere);
	}

	void ShootSphere(int x, int y)
	{
		if (m_spheres.size() > 0)
		{
			Sphere * pSphere = *(m_spheres.begin());

			pSphere->Position = m_pSceneRenderer->UnprojectScreenCoordinateAt(x, y);
			pSphere->Speed = glm::vec3(0, 0, -1);
		}
	}
};
