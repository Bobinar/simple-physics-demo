#pragma once

#include <SceneRenderer.h>
#include <SceneSimulator.h>
#include <Sphere.h>

#include <vector>
#include <memory>

class SceneManager
{
	SceneRenderer * m_pSceneRenderer;
	SceneSimulator * m_pSceneSimulator;
	std::vector<std::unique_ptr<Sphere>> m_spheres;

public:
	SceneManager(SceneRenderer *pSceneRenderer, SceneSimulator *pSceneSimulator);
	~SceneManager();
	void Update(float deltaTime);	void Draw();
	void AddSphere(std::unique_ptr<Sphere> pSphere);
	void ShootSphere(int x, int y);
};
