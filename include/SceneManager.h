#pragma once

#include <SceneRenderer.h>
#include <SceneSimulator.h>

#include <vector>

class SceneManager
{
	SceneRenderer * m_pSceneRenderer;
	SceneSimulator * m_pSceneSimulator;
	std::vector<Sphere*> m_spheres;

public:
	SceneManager(SceneRenderer *pSceneRenderer, SceneSimulator *pSceneSimulator);
	~SceneManager();
	void Update(float deltaTime);	void Draw();
	void AddSphere(Sphere * pSphere);
	void ShootSphere(int x, int y);
};
